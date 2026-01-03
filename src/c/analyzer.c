#include "analyzer.h"
#include "dictionary.h"
#include "utils.h"
#include "morphology.h"
#include "json_loader.h"
#include "morphophonemic.h"
#include "reduplication.h"
#include "queue.h"
#include "hashset.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// =====================================================
// Internal State Management
// =====================================================

static bool g_analyzer_initialized = false;

// Create a new analysis state
static AnalysisState* create_state(AnalysisStage stage, const char* remaining, 
                                    const char* original) {
    AnalysisState* state = (AnalysisState*)calloc(1, sizeof(AnalysisState));
    if (!state) return NULL;
    
    state->stage = stage;
    safe_strcpy(state->remaining, remaining, sizeof(state->remaining));
    safe_strcpy(state->original, original, sizeof(state->original));
    state->affix_count = 0;
    state->morpho_changes[0] = '\0';
    state->redup_type = REDUP_NONE;
    state->redup_removed[0] = '\0';
    state->has_circumfix_start = false;
    state->expected_suffix[0] = '\0';
    state->pending_circumfix = NULL;
    state->circumfix_prefix_index = -1;
    
    return state;
}

// Clone an analysis state
static AnalysisState* clone_state(const AnalysisState* src) {
    if (!src) return NULL;
    
    AnalysisState* state = (AnalysisState*)malloc(sizeof(AnalysisState));
    if (!state) return NULL;
    
    memcpy(state, src, sizeof(AnalysisState));
    return state;
}

// Free an analysis state
static void free_state(AnalysisState* state) {
    if (state) {
        free(state);
    }
}

// Generate state key for visited tracking
static void generate_state_key(const AnalysisState* state, char* key, size_t key_size) {
    snprintf(key, key_size, "%s|%d|%d", 
             state->remaining, 
             state->stage, 
             state->affix_count);
}

// Add trace message to result
static void add_trace(AnalysisResult* result, const char* msg) {
    if (result->trace_count < 16) {
        safe_strcpy(result->trace[result->trace_count], msg, 256);
        result->trace_count++;
    }
}

// Create analysis result from state
static AnalysisResult* create_result_from_state(const AnalysisState* state, 
                                                 const char* root,
                                                 const char* root_gloss,
                                                 Language lang) {
    AnalysisResult* result = (AnalysisResult*)calloc(1, sizeof(AnalysisResult));
    if (!result) return NULL;
    
    result->valid = true;
    safe_strcpy(result->original, state->original, sizeof(result->original));
    safe_strcpy(result->root, root, sizeof(result->root));
    if (root_gloss) {
        safe_strcpy(result->root_gloss, root_gloss, sizeof(result->root_gloss));
    }
    result->language = lang;
    
    // Copy affixes
    result->affix_count = state->affix_count;
    for (int i = 0; i < state->affix_count; i++) {
        result->affixes[i] = state->affixes[i];
    }
    
    // Copy reduplication info
    result->redup_type = state->redup_type;
    safe_strcpy(result->redup_pattern, redup_type_to_string(state->redup_type), 
                sizeof(result->redup_pattern));
    
    // Copy morphophonemic changes
    safe_strcpy(result->morpho_changes, state->morpho_changes, 
                sizeof(result->morpho_changes));
    
    // Generate structure string
    // e.g., "[gi-] + palit + [-an]"
    char structure[512] = "";
    
    // Add prefixes, infixes, and circumfix starts
    for (int i = 0; i < result->affix_count; i++) {
        if (result->affixes[i].position == POS_PREFIX) {
            char affix_str[64];
            snprintf(affix_str, sizeof(affix_str), "[%s] + ", 
                     result->affixes[i].matched_form);
            strcat(structure, affix_str);
        } else if (result->affixes[i].position == POS_INFIX) {
            char affix_str[64];
            snprintf(affix_str, sizeof(affix_str), "[%s] + ", 
                     result->affixes[i].matched_form);
            strcat(structure, affix_str);
        } else if (result->affixes[i].position == POS_CIRCUMFIX) {
            char affix_str[64];
            snprintf(affix_str, sizeof(affix_str), "[%s] + ", 
                     result->affixes[i].affix->prefix_part);
            strcat(structure, affix_str);
        }
    }
    
    // Add reduplication if present
    if (result->redup_type != REDUP_NONE) {
        char redup_str[64];
        snprintf(redup_str, sizeof(redup_str), "[%s-REDUP] + ", result->redup_pattern);
        strcat(structure, redup_str);
    }
    
    // Add root
    strcat(structure, root);
    
    // Add suffixes and circumfix ends
    for (int i = 0; i < result->affix_count; i++) {
        if (result->affixes[i].position == POS_SUFFIX) {
            char affix_str[64];
            snprintf(affix_str, sizeof(affix_str), " + [%s]", 
                     result->affixes[i].matched_form);
            strcat(structure, affix_str);
        } else if (result->affixes[i].position == POS_CIRCUMFIX) {
            char affix_str[64];
            snprintf(affix_str, sizeof(affix_str), " + [%s]", 
                     result->affixes[i].affix->suffix_part);
            strcat(structure, affix_str);
        }
    }
    
    safe_strcpy(result->structure, structure, sizeof(result->structure));
    
    // Calculate confidence score
    result->confidence_score = 1.0f;
    // Reduce confidence for each morphophonemic change
    int change_count = 0;
    for (size_t i = 0; i < strlen(result->morpho_changes); i++) {
        if (result->morpho_changes[i] == ';') change_count++;
    }
    result->confidence_score -= change_count * 0.1f;
    if (result->confidence_score < 0.1f) result->confidence_score = 0.1f;
    
    result->next = NULL;
    
    // Add trace message
    char trace_msg[256];
    snprintf(trace_msg, sizeof(trace_msg), "Found root: %s (%s)", 
             root, language_to_string(lang));
    add_trace(result, trace_msg);
    
    return result;
}

// =====================================================
// Affix Iteration Helpers
// =====================================================

// Structure for iterating through all affixes in a table
typedef struct AffixIterator {
    AffixTable* table;
    unsigned int bucket_index;
    Affix* current_affix;
} AffixIterator;

static void affix_iter_init(AffixIterator* iter, AffixTable* table) {
    iter->table = table;
    iter->bucket_index = 0;
    iter->current_affix = NULL;
    
    // Find first non-empty bucket
    if (table) {
        for (unsigned int i = 0; i < table->size; i++) {
            if (table->buckets[i]) {
                iter->bucket_index = i;
                iter->current_affix = table->buckets[i];
                break;
            }
        }
    }
}

static Affix* affix_iter_next(AffixIterator* iter) {
    if (!iter || !iter->table || !iter->current_affix) return NULL;
    
    Affix* result = iter->current_affix;
    
    // Move to next
    if (iter->current_affix->next) {
        iter->current_affix = iter->current_affix->next;
    } else {
        // Find next non-empty bucket
        iter->current_affix = NULL;
        for (unsigned int i = iter->bucket_index + 1; i < iter->table->size; i++) {
            if (iter->table->buckets[i]) {
                iter->bucket_index = i;
                iter->current_affix = iter->table->buckets[i];
                break;
            }
        }
    }
    
    return result;
}

// =====================================================
// Stage Processing Functions
// =====================================================

// Process prefix stage
static void process_prefix_stage(AnalysisState* current, Queue* q, 
                                  HashSet* visited, const char* language) {
    (void)visited;  // Reserved for future use
    bool found_any = false;
    
    // First, check for reduplication (before prefix stripping)
    ReduplicationList* redups = detect_reduplication(current->remaining, language);
    if (redups && redups->count > 0) {
        for (int i = 0; i < redups->count; i++) {
            AnalysisState* new_state = clone_state(current);
            if (new_state) {
                safe_strcpy(new_state->remaining, redups->patterns[i].base, 
                           sizeof(new_state->remaining));
                new_state->redup_type = redups->patterns[i].type;
                safe_strcpy(new_state->redup_removed, redups->patterns[i].removed,
                           sizeof(new_state->redup_removed));
                
                // Stay in prefix check stage (might have prefix after reduplication)
                queue_enqueue(q, new_state);
                found_any = true;
            }
        }
    }
    free_reduplication_list(redups);
    
    // Try all prefixes
    AffixIterator iter;
    affix_iter_init(&iter, g_prefix_table);
    
    Affix* prefix;
    while ((prefix = affix_iter_next(&iter)) != NULL) {
        // Check if this prefix matches language
        if (!affix_matches_language(prefix, language)) continue;
        
        // Get the prefix key (without trailing hyphen for matching)
        char prefix_form[64];
        safe_strcpy(prefix_form, prefix->key, sizeof(prefix_form));
        
        // Remove trailing hyphen for matching
        size_t plen = strlen(prefix_form);
        if (plen > 0 && prefix_form[plen-1] == '-') {
            prefix_form[plen-1] = '\0';
            plen--;
        }
        
        // Check if word starts with this prefix
        if (starts_with(current->remaining, prefix_form)) {
            // Strip prefix
            const char* after_prefix = current->remaining + plen;
            
            // Apply morphophonemic reversal
            MorphoCandidateList* candidates = reverse_morphophonemic(
                after_prefix, prefix->key, language);
            
            if (candidates) {
                for (int i = 0; i < candidates->count; i++) {
                    AnalysisState* new_state = clone_state(current);
                    if (new_state) {
                        safe_strcpy(new_state->remaining, candidates->candidates[i].form,
                                   sizeof(new_state->remaining));
                        
                        // Add this prefix to affixes
                        if (new_state->affix_count < MAX_AFFIXES) {
                            MatchedAffix* ma = &new_state->affixes[new_state->affix_count];
                            ma->affix = prefix;
                            ma->definition = prefix->definitions;  // First definition
                            safe_strcpy(ma->matched_form, prefix->key, sizeof(ma->matched_form));
                            ma->position = POS_PREFIX;
                            new_state->affix_count++;
                        }
                        
                        // Append morphophonemic changes
                        if (strlen(candidates->candidates[i].changes) > 0) {
                            strcat(new_state->morpho_changes, candidates->candidates[i].changes);
                            strcat(new_state->morpho_changes, ";");
                        }
                        
                        // Check for circumfix start
                        // Look for matching circumfix in circumfix table
                        AffixIterator cfix_iter;
                        affix_iter_init(&cfix_iter, g_circumfix_table);
                        Affix* cfix;
                        while ((cfix = affix_iter_next(&cfix_iter)) != NULL) {
                            // Check if circumfix prefix part matches the found prefix
                            // Note: prefix_form has trailing hyphen removed
                            // cfix->prefix_part usually has trailing hyphen (e.g., "ka-")
                            // We check if cfix->prefix_part starts with prefix_form
                            if (starts_with(cfix->prefix_part, prefix_form)) {
                                AnalysisState* cfix_state = clone_state(new_state);
                                if (cfix_state) {
                                    cfix_state->has_circumfix_start = true;
                                    safe_strcpy(cfix_state->expected_suffix, cfix->suffix_part,
                                               sizeof(cfix_state->expected_suffix));
                                    cfix_state->pending_circumfix = cfix;
                                    cfix_state->circumfix_prefix_index = cfix_state->affix_count - 1;
                                    
                                    cfix_state->stage = STAGE_PREFIX_CHECK;
                                    queue_enqueue(q, cfix_state);
                                }
                            }
                        }
                        
                        // Always enqueue the state as valid Prefix-only path too
                        // Could have nested prefixes, stay in prefix stage
                        new_state->stage = STAGE_PREFIX_CHECK;
                        queue_enqueue(q, new_state);
                        found_any = true;
                    }
                }
                free_morpho_candidates(candidates);
            }
        }
    }
    
    // Also try moving to infix check without finding a prefix
    AnalysisState* no_prefix_state = clone_state(current);
    if (no_prefix_state) {
        no_prefix_state->stage = STAGE_INFIX_CHECK;
        queue_enqueue(q, no_prefix_state);
    }
    
    (void)found_any;  // Suppress unused-but-set warning
}

// Process infix stage
static void process_infix_stage(AnalysisState* current, Queue* q, 
                                 HashSet* visited, const char* language) {
    (void)visited;  // Reserved for future use
    bool found_any = false;
    
    AffixIterator iter;
    affix_iter_init(&iter, g_infix_table);
    
    Affix* infix;
    while ((infix = affix_iter_next(&iter)) != NULL) {
        if (!affix_matches_language(infix, language)) continue;
        
        // Get infix form (remove surrounding hyphens)
        char infix_form[32];
        safe_strcpy(infix_form, infix->key, sizeof(infix_form));
        
        // Remove leading and trailing hyphens
        char* start = infix_form;
        if (*start == '-') start++;
        size_t len = strlen(start);
        if (len > 0 && start[len-1] == '-') {
            start[len-1] = '\0';
            len--;
        }
        
        // Find infix in word (should be after first consonant)
        const char* remaining = current->remaining;
        size_t rem_len = strlen(remaining);
        
        // Typical infix position: after first consonant
        // e.g., s-um-ulat, t-in-ahi
        if (rem_len > len + 1 && is_consonant(remaining[0])) {
            // Check if infix appears after first consonant
            if (strncmp(remaining + 1, start, len) == 0) {
                // Found infix at position 1
                // Reconstruct: first_char + rest_after_infix
                char stripped[256];
                stripped[0] = remaining[0];
                strcpy(stripped + 1, remaining + 1 + len);
                
                AnalysisState* new_state = clone_state(current);
                if (new_state) {
                    safe_strcpy(new_state->remaining, stripped, sizeof(new_state->remaining));
                    
                    if (new_state->affix_count < MAX_AFFIXES) {
                        MatchedAffix* ma = &new_state->affixes[new_state->affix_count];
                        ma->affix = infix;
                        ma->definition = infix->definitions;
                        safe_strcpy(ma->matched_form, infix->key, sizeof(ma->matched_form));
                        ma->position = POS_INFIX;
                        new_state->affix_count++;
                    }
                    
                    new_state->stage = STAGE_SUFFIX_CHECK;
                    queue_enqueue(q, new_state);
                    found_any = true;
                }
            }
        }
    }
    
    // Move to suffix check even if no infix found
    AnalysisState* no_infix_state = clone_state(current);
    if (no_infix_state) {
        no_infix_state->stage = STAGE_SUFFIX_CHECK;
        queue_enqueue(q, no_infix_state);
    }
    
    (void)found_any;  // Suppress unused-but-set warning
}

// Process suffix stage
static void process_suffix_stage(AnalysisState* current, Queue* q, 
                                  HashSet* visited, const char* language) {
    (void)visited;  // Reserved for future use
    bool found_any = false;
    
    AffixIterator iter;
    affix_iter_init(&iter, g_suffix_table);
    
    Affix* suffix;
    while ((suffix = affix_iter_next(&iter)) != NULL) {
        if (!affix_matches_language(suffix, language)) continue;
        
        // Get suffix form (remove leading hyphen)
        char suffix_form[64];
        safe_strcpy(suffix_form, suffix->key, sizeof(suffix_form));
        
        char* start = suffix_form;
        if (*start == '-') start++;
        
        // Check if word ends with this suffix
        if (ends_with(current->remaining, start)) {
            // Check circumfix completion
            if (current->has_circumfix_start) {
                // Verify this suffix completes the circumfix
                char expected[32];
                safe_strcpy(expected, current->expected_suffix, sizeof(expected));
                if (expected[0] == '-') {
                    if (strcmp(expected + 1, start) != 0) {
                        continue;  // Wrong suffix for this circumfix
                    }
                }
            }
            
            // Strip suffix
            size_t suffix_len = strlen(start);
            size_t remaining_len = strlen(current->remaining);
            
            if (remaining_len > suffix_len) {
                char stripped[256];
                if (remaining_len - suffix_len < sizeof(stripped)) {
                    memcpy(stripped, current->remaining, remaining_len - suffix_len);
                    stripped[remaining_len - suffix_len] = '\0';
                } else {
                     continue; // Skip if too long, safety check
                }
                stripped[remaining_len - suffix_len] = '\0';
                
                // Apply morphophonemic reversal
                MorphoCandidateList* candidates = reverse_morphophonemic(
                    stripped, suffix->key, language);
                
                if (candidates) {
                    for (int i = 0; i < candidates->count; i++) {
                        AnalysisState* new_state = clone_state(current);
                        if (new_state) {
                            safe_strcpy(new_state->remaining, candidates->candidates[i].form,
                                       sizeof(new_state->remaining));
                            
                            if (new_state->has_circumfix_start && new_state->pending_circumfix && 
                                new_state->circumfix_prefix_index >= 0 && 
                                new_state->circumfix_prefix_index < new_state->affix_count) {
                                // Conversion to circumfix: Update the prefix entry to be the circumfix
                                MatchedAffix* ma = &new_state->affixes[new_state->circumfix_prefix_index];
                                ma->affix = new_state->pending_circumfix;
                                ma->definition = new_state->pending_circumfix->definitions;
                                safe_strcpy(ma->matched_form, new_state->pending_circumfix->key, sizeof(ma->matched_form));
                                ma->position = POS_CIRCUMFIX;
                                
                                // Reset circumfix state (we consumed it)
                                new_state->has_circumfix_start = false;
                                new_state->pending_circumfix = NULL;
                            } else if (new_state->affix_count < MAX_AFFIXES) {
                                MatchedAffix* ma = &new_state->affixes[new_state->affix_count];
                                ma->affix = suffix;
                                ma->definition = suffix->definitions;
                                safe_strcpy(ma->matched_form, suffix->key, sizeof(ma->matched_form));
                                ma->position = POS_SUFFIX;
                                new_state->affix_count++;
                            }
                            
                            if (strlen(candidates->candidates[i].changes) > 0) {
                                strcat(new_state->morpho_changes, candidates->candidates[i].changes);
                                strcat(new_state->morpho_changes, ";");
                            }
                            
                            // Could have nested suffixes
                            new_state->stage = STAGE_SUFFIX_CHECK;
                            queue_enqueue(q, new_state);
                            found_any = true;
                        }
                    }
                    free_morpho_candidates(candidates);
                }
            }
        }
    }
    
    // Move to root check
    AnalysisState* root_state = clone_state(current);
    if (root_state) {
        root_state->stage = STAGE_ROOT_CHECK;
        queue_enqueue(q, root_state);
    }
    
    (void)found_any;  // Suppress unused-but-set warning
}

// External root lookup (from dictionary.c - legacy)
extern HashTable* bisaya_roots;
extern HashTable* tagalog_roots;


// Process root stage
static void process_root_stage(AnalysisState* current, AnalysisResult** results,
                                const char* language) {
    const char* remaining = current->remaining;
    
    // Minimum root length
    if (strlen(remaining) < 2) return;
    
    // Try to find in root dictionaries using new JSON loader
    RootWord* root_word = NULL;
    Language detected_lang = LANG_UNKNOWN;
    const char* root_gloss = NULL;
    
    // Try the new JSON-based lookup first
    root_word = lookup_any_root(remaining, language);
    
    if (root_word) {
        root_gloss = root_word->definition;
        
        // Determine language from root
        if (strcmp(root_word->language, "Hiligaynon") == 0 || 
            strcmp(root_word->language, "Cebuano") == 0 ||
            strcmp(root_word->language, "Bisaya") == 0) {
            detected_lang = LANG_BISAYA;
        } else if (strcmp(root_word->language, "Tagalog") == 0) {
            detected_lang = LANG_TAGALOG;
        } else {
            detected_lang = LANG_BOTH;
        }
    } else {
        // Fallback to legacy hash table lookup
        char* root_val = NULL;
        
        // Check Cebuano roots
        if (bisaya_roots && (!language || strcmp(language, "Cebuano") == 0 || 
                              strcmp(language, "Bisaya") == 0 ||
                              strcmp(language, "Both") == 0)) {
            root_val = ht_lookup(bisaya_roots, remaining);
            if (root_val) {
                root_gloss = root_val;
                detected_lang = LANG_BISAYA;
            }
        }
        
        // Check Tagalog roots
        if (!root_val && tagalog_roots && (!language || strcmp(language, "Tagalog") == 0 || 
                                            strcmp(language, "Both") == 0)) {
            root_val = ht_lookup(tagalog_roots, remaining);
            if (root_val) {
                root_gloss = root_val;
                detected_lang = LANG_TAGALOG;
            }
        }
        

    }
    
    // If we found a root, create a result
    if (root_word || root_gloss) {
        AnalysisResult* result = create_result_from_state(
            current, remaining, root_gloss ? root_gloss : "", detected_lang);
        
        if (result) {
            // Add to results list
            result->next = *results;
            *results = result;
        }
    }
    
    // Even if not in dictionary, if we stripped affixes, might still be valid
    // This allows analyzing words with unknown roots
    if (!root_word && !root_gloss && current->affix_count > 0) {
        AnalysisResult* result = create_result_from_state(
            current, remaining, "[unknown root]", 
            language ? parse_language(language) : LANG_UNKNOWN);
        
        if (result) {
            result->confidence_score *= 0.5f;  // Lower confidence for unknown roots
            result->next = *results;
            *results = result;
        }
    }
}


// =====================================================
// Public API Implementation
// =====================================================

bool init_analyzer_v2(const char* data_dir) {
    if (g_analyzer_initialized) {
        return true;
    }
    
    // Load affix tables
    if (!load_affix_tables(data_dir)) {
        fprintf(stderr, "Warning: Some affix tables failed to load\n");
    }
    
    // Load root dictionaries from new JSON format
    if (!load_root_dictionaries(data_dir)) {
        fprintf(stderr, "Warning: Some root dictionaries failed to load\n");
    }
    
    g_analyzer_initialized = true;
    
    // Print loading stats
    print_affix_stats();
    print_root_stats();
    
    return true;

}

AnalysisResult* analyze_word_v2(const char* word, const char* language) {
    if (!word || strlen(word) == 0) return NULL;
    
    // Normalize word
    char normalized[MAX_WORD_LEN];
    normalize_word(word, normalized, sizeof(normalized));
    
    if (strlen(normalized) < 2) return NULL;
    
    // Initialize BFS
    Queue* q = queue_create();
    HashSet* visited = hashset_create(256);
    AnalysisResult* results = NULL;
    
    if (!q || !visited) {
        if (q) queue_destroy(q);
        if (visited) hashset_destroy(visited);
        return NULL;
    }
    
    // Create initial state
    AnalysisState* initial = create_state(STAGE_PREFIX_CHECK, normalized, word);
    if (!initial) {
        queue_destroy(q);
        hashset_destroy(visited);
        return NULL;
    }
    
    queue_enqueue(q, initial);
    
    // BFS loop
    int iteration = 0;
    const int MAX_ITERATIONS = 1000;  // Safety limit
    
    while (!queue_is_empty(q) && iteration < MAX_ITERATIONS) {
        iteration++;
        
        AnalysisState* current = (AnalysisState*)queue_dequeue(q);
        if (!current) continue;
        
        // Generate state key for visited check
        char state_key[512];
        generate_state_key(current, state_key, sizeof(state_key));
        
        // Skip if already visited
        if (hashset_contains(visited, state_key)) {
            free_state(current);
            continue;
        }
        hashset_add(visited, state_key);
        
        // Process based on stage
        switch (current->stage) {
            case STAGE_PREFIX_CHECK:
                process_prefix_stage(current, q, visited, language);
                break;
                
            case STAGE_INFIX_CHECK:
                process_infix_stage(current, q, visited, language);
                break;
                
            case STAGE_SUFFIX_CHECK:
                process_suffix_stage(current, q, visited, language);
                break;
                
            case STAGE_ROOT_CHECK:
                process_root_stage(current, &results, language);
                break;
                
            default:
                break;
        }
        
        free_state(current);
    }
    
    // Cleanup
    // Free any remaining states in queue
    while (!queue_is_empty(q)) {
        AnalysisState* state = (AnalysisState*)queue_dequeue(q);
        free_state(state);
    }
    queue_destroy(q);
    hashset_destroy(visited);
    
    return results;
}

void free_analysis_result(AnalysisResult* result) {
    if (result) {
        free(result);
    }
}

void free_analysis_results(AnalysisResult* results) {
    while (results) {
        AnalysisResult* next = results->next;
        free_analysis_result(results);
        results = next;
    }
}

int count_analyses(AnalysisResult* results) {
    int count = 0;
    while (results) {
        count++;
        results = results->next;
    }
    return count;
}

void print_analysis_result(const AnalysisResult* result) {
    if (!result) return;
    
    printf("----------------------------------------\n");
    printf("Original: %s\n", result->original);
    printf("Root: %s\n", result->root);
    printf("Language: %s\n", language_to_string(result->language));
    printf("Structure: %s\n", result->structure);
    printf("Confidence: %.2f\n", result->confidence_score);
    
    if (result->affix_count > 0) {
        printf("Affixes (%d):\n", result->affix_count);
        for (int i = 0; i < result->affix_count; i++) {
            const MatchedAffix* ma = &result->affixes[i];
            printf("  - %s (%s)\n", ma->matched_form, 
                   ma->position == POS_PREFIX ? "prefix" :
                   ma->position == POS_SUFFIX ? "suffix" :
                   ma->position == POS_INFIX ? "infix" : "circumfix");
            if (ma->definition && ma->definition->function[0]) {
                printf("    Function: %s\n", ma->definition->function);
            }
        }
    }
    
    if (result->redup_type != REDUP_NONE) {
        printf("Reduplication: %s\n", result->redup_pattern);
    }
    
    if (strlen(result->morpho_changes) > 0) {
        printf("Morphophonemic: %s\n", result->morpho_changes);
    }
}

void print_all_analyses(const AnalysisResult* results) {
    printf("========================================\n");
    printf("          ANALYSIS RESULTS             \n");
    printf("========================================\n");
    
    int count = 0;
    while (results) {
        count++;
        printf("\n--- Analysis #%d ---\n", count);
        print_analysis_result(results);
        results = results->next;
    }
    
    printf("\n========================================\n");
    printf("Total analyses: %d\n", count);
    printf("========================================\n");
}

void cleanup_analyzer_v2(void) {
    cleanup_affix_tables();
    cleanup_root_dictionaries();
    g_analyzer_initialized = false;
}

// =====================================================
// Legacy API (Backward Compatibility)
// =====================================================

// These use the old hash tables from the original implementation
static HashTable* legacy_prefix_table = NULL;
static HashTable* legacy_suffix_table = NULL;

// Forward declaration for external hash tables
HashTable* bisaya_roots = NULL;
HashTable* tagalog_roots = NULL;


static void populate_table(HashTable* table, const char* data) {
    if (!table || !data) return;
    char* data_copy = my_strdup(data);
    char* line = strtok(data_copy, "\n");
    while (line) {
        char* sep = strchr(line, '|');
        if (sep) {
            *sep = '\0';
            char* key = line;
            char* val = sep + 1;
            ht_insert(table, key, val);
        }
        line = strtok(NULL, "\n");
    }
    free(data_copy);
}

void init_analyzer(const char* prefix_data, const char* suffix_data,
                   const char* bisaya_data, const char* tagalog_data,
                   const char* shared_data) {
    legacy_prefix_table = ht_create(50);
    legacy_suffix_table = ht_create(50);
    bisaya_roots = ht_create(250);
    tagalog_roots = ht_create(250);

    populate_table(legacy_prefix_table, prefix_data);
    populate_table(legacy_suffix_table, suffix_data);
    populate_table(bisaya_roots, bisaya_data);
    populate_table(tagalog_roots, tagalog_data);

}

void analyze_word(const char* word, LegacyAnalysisResult* res) {
    memset(res, 0, sizeof(LegacyAnalysisResult));
    safe_strcpy(res->word, word, MAX_WORD_LEN);
    res->valid = false;
    res->language = LANG_UNKNOWN;
    
    // Use new analyzer if initialized
    if (g_analyzer_initialized) {
        AnalysisResult* results = analyze_word_v2(word, NULL);
        if (results && results->valid) {
            res->valid = true;
            safe_strcpy(res->root, results->root, 128);
            res->language = results->language;
            
            // Extract prefix and suffix
            for (int i = 0; i < results->affix_count; i++) {
                if (results->affixes[i].position == POS_PREFIX && res->prefix[0] == '\0') {
                    safe_strcpy(res->prefix, results->affixes[i].matched_form, 64);
                }
                if (results->affixes[i].position == POS_SUFFIX && res->suffix[0] == '\0') {
                    safe_strcpy(res->suffix, results->affixes[i].matched_form, 64);
                }
            }
            
            // Add trace
            res->trace_count = results->trace_count;
            for (int i = 0; i < results->trace_count && i < 10; i++) {
                safe_strcpy(res->trace[i], results->trace[i], 256);
            }
        }
        free_analysis_results(results);
        return;
    }
    
    // Fallback to simple analysis (original code path)
    // ... (simplified - just indicate not found)
    res->valid = false;
}

void cleanup_analyzer(void) {
    if (legacy_prefix_table) ht_destroy(legacy_prefix_table);
    if (legacy_suffix_table) ht_destroy(legacy_suffix_table);
    if (bisaya_roots) ht_destroy(bisaya_roots);
    if (tagalog_roots) ht_destroy(tagalog_roots);

    
    legacy_prefix_table = NULL;
    legacy_suffix_table = NULL;
    bisaya_roots = NULL;
    tagalog_roots = NULL;

    
    cleanup_analyzer_v2();
} 
 
 / /   = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  
 / /   D e t e r m i n i s t i c   F i n i t e   A u t o m a t o n   I m p l e m e n t a t i o n   ( G r e e d y )  
 / /   = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  
  
 / /   H e l p e r   s t r u c t   f o r   f i n d i n g   b e s t   m a t c h  
 t y p e d e f   s t r u c t   B e s t M a t c h   {  
         A f f i x *   a f f i x ;  
         s i z e _ t   l e n ;  
         c h a r   m a t c h e d _ f o r m [ 6 4 ] ;  
         M o r p h o C a n d i d a t e   c a n d i d a t e ;  
 }   B e s t M a t c h ;  
  
 / /   F i n d   l o n g e s t   m a t c h i n g   p r e f i x  
 s t a t i c   b o o l   f i n d _ b e s t _ p r e f i x ( c o n s t   c h a r *   w o r d ,   c o n s t   c h a r *   l a n g u a g e ,   B e s t M a t c h *   b e s t )   {  
         i f   ( ! w o r d   | |   ! b e s t )   r e t u r n   f a l s e ;  
          
         b e s t - > a f f i x   =   N U L L ;  
         b e s t - > l e n   =   0 ;  
          
         A f f i x I t e r a t o r   i t e r ;  
         a f f i x _ i t e r _ i n i t ( & i t e r ,   g _ p r e f i x _ t a b l e ) ;  
          
         A f f i x *   a f f i x ;  
         w h i l e   ( ( a f f i x   =   a f f i x _ i t e r _ n e x t ( & i t e r ) )   ! =   N U L L )   {  
                 i f   ( ! a f f i x _ m a t c h e s _ l a n g u a g e ( a f f i x ,   l a n g u a g e ) )   c o n t i n u e ;  
                  
                 c h a r   p r e f i x _ f o r m [ 6 4 ] ;  
                 s a f e _ s t r c p y ( p r e f i x _ f o r m ,   a f f i x - > k e y ,   s i z e o f ( p r e f i x _ f o r m ) ) ;  
                  
                 s i z e _ t   p l e n   =   s t r l e n ( p r e f i x _ f o r m ) ;  
                 i f   ( p l e n   >   0   & &   p r e f i x _ f o r m [ p l e n - 1 ]   = =   ' - ' )   {  
                         p r e f i x _ f o r m [ p l e n - 1 ]   =   ' \ 0 ' ;  
                         p l e n - - ;  
                 }  
                  
                 i f   ( s t a r t s _ w i t h ( w o r d ,   p r e f i x _ f o r m ) )   {  
                         / /   A p p l y   m o r p h o p h o n e m i c   c h e c k   i m m e d i a t e l y   t o   v e r i f y   v a l i d i t y  
                         c o n s t   c h a r *   a f t e r _ p r e f i x   =   w o r d   +   p l e n ;  
                         M o r p h o C a n d i d a t e L i s t *   c a n d i d a t e s   =   r e v e r s e _ m o r p h o p h o n e m i c (  
                                 a f t e r _ p r e f i x ,   a f f i x - > k e y ,   l a n g u a g e ) ;  
                                  
                         i f   ( c a n d i d a t e s   & &   c a n d i d a t e s - > c o u n t   >   0 )   {  
                                 / /   I f   t h i s   i s   l o n g e r   t h a n   c u r r e n t   b e s t ,   r e p l a c e   i t  
                                 i f   ( p l e n   >   b e s t - > l e n )   {  
                                         b e s t - > a f f i x   =   a f f i x ;  
                                         b e s t - > l e n   =   p l e n ;  
                                         s a f e _ s t r c p y ( b e s t - > m a t c h e d _ f o r m ,   a f f i x - > k e y ,   s i z e o f ( b e s t - > m a t c h e d _ f o r m ) ) ;  
                                         / /   T a k e   f i r s t   v a l i d   c a n d i d a t e   ( G r e e d y )  
                                         b e s t - > c a n d i d a t e   =   c a n d i d a t e s - > c a n d i d a t e s [ 0 ] ;  
                                 }  
                                 f r e e _ m o r p h o _ c a n d i d a t e s ( c a n d i d a t e s ) ;  
                         }  
                 }  
         }  
          
         r e t u r n   b e s t - > a f f i x   ! =   N U L L ;  
 }  
  
 / /   F i n d   l o n g e s t   m a t c h i n g   s u f f i x  
 s t a t i c   b o o l   f i n d _ b e s t _ s u f f i x ( c o n s t   c h a r *   w o r d ,   c o n s t   c h a r *   l a n g u a g e ,    
                                                             B e s t M a t c h *   b e s t ,   A n a l y s i s S t a t e *   s t a t e )   {  
         i f   ( ! w o r d   | |   ! b e s t )   r e t u r n   f a l s e ;  
          
         b e s t - > a f f i x   =   N U L L ;  
         b e s t - > l e n   =   0 ;  
          
         A f f i x I t e r a t o r   i t e r ;  
         a f f i x _ i t e r _ i n i t ( & i t e r ,   g _ s u f f i x _ t a b l e ) ;  
          
         A f f i x *   a f f i x ;  
         w h i l e   ( ( a f f i x   =   a f f i x _ i t e r _ n e x t ( & i t e r ) )   ! =   N U L L )   {  
                 i f   ( ! a f f i x _ m a t c h e s _ l a n g u a g e ( a f f i x ,   l a n g u a g e ) )   c o n t i n u e ;  
                  
                 c h a r   s u f f i x _ f o r m [ 6 4 ] ;  
                 s a f e _ s t r c p y ( s u f f i x _ f o r m ,   a f f i x - > k e y ,   s i z e o f ( s u f f i x _ f o r m ) ) ;  
                  
                 c h a r *   s t a r t   =   s u f f i x _ f o r m ;  
                 i f   ( * s t a r t   = =   ' - ' )   s t a r t + + ;  
                 s i z e _ t   s l e n   =   s t r l e n ( s t a r t ) ;  
                  
                 i f   ( e n d s _ w i t h ( w o r d ,   s t a r t ) )   {  
                         / /   C h e c k   c i r c u m f i x   c o n s t r a i n t  
                         i f   ( s t a t e - > h a s _ c i r c u m f i x _ s t a r t )   {  
                                 c h a r   e x p e c t e d [ 3 2 ] ;  
                                 s a f e _ s t r c p y ( e x p e c t e d ,   s t a t e - > e x p e c t e d _ s u f f i x ,   s i z e o f ( e x p e c t e d ) ) ;  
                                 i f   ( e x p e c t e d [ 0 ]   = =   ' - ' )   {  
                                           i f   ( s t r c m p ( e x p e c t e d   +   1 ,   s t a r t )   ! =   0 )   c o n t i n u e ;  
                                 }  
                         }  
                  
                         s i z e _ t   w o r d _ l e n   =   s t r l e n ( w o r d ) ;  
                         i f   ( w o r d _ l e n   >   s l e n )   {  
                                 c h a r   s t r i p p e d [ 2 5 6 ] ;  
                                 s i z e _ t   s t e m _ l e n   =   w o r d _ l e n   -   s l e n ;  
                                 i f   ( s t e m _ l e n   <   s i z e o f ( s t r i p p e d ) )   {  
                                         m e m c p y ( s t r i p p e d ,   w o r d ,   s t e m _ l e n ) ;  
                                         s t r i p p e d [ s t e m _ l e n ]   =   ' \ 0 ' ;  
                                          
                                         M o r p h o C a n d i d a t e L i s t *   c a n d i d a t e s   =   r e v e r s e _ m o r p h o p h o n e m i c (  
                                                 s t r i p p e d ,   a f f i x - > k e y ,   l a n g u a g e ) ;  
                                                  
                                         i f   ( c a n d i d a t e s   & &   c a n d i d a t e s - > c o u n t   >   0 )   {  
                                                 i f   ( s l e n   >   b e s t - > l e n )   {  
                                                         b e s t - > a f f i x   =   a f f i x ;  
                                                         b e s t - > l e n   =   s l e n ;  
                                                         s a f e _ s t r c p y ( b e s t - > m a t c h e d _ f o r m ,   a f f i x - > k e y ,   s i z e o f ( b e s t - > m a t c h e d _ f o r m ) ) ;  
                                                         b e s t - > c a n d i d a t e   =   c a n d i d a t e s - > c a n d i d a t e s [ 0 ] ;  
                                                 }  
                                                 f r e e _ m o r p h o _ c a n d i d a t e s ( c a n d i d a t e s ) ;  
                                         }  
                                 }  
                         }  
                 }  
         }  
          
         r e t u r n   b e s t - > a f f i x   ! =   N U L L ;  
 }  
  
 A n a l y s i s R e s u l t *   a n a l y z e _ w o r d _ d e t e r m i n i s t i c ( c o n s t   c h a r *   w o r d ,   c o n s t   c h a r *   l a n g u a g e )   {  
         i f   ( ! w o r d   | |   s t r l e n ( w o r d )   = =   0 )   r e t u r n   N U L L ;  
          
         c h a r   n o r m a l i z e d [ M A X _ W O R D _ L E N ] ;  
         n o r m a l i z e _ w o r d ( w o r d ,   n o r m a l i z e d ,   s i z e o f ( n o r m a l i z e d ) ) ;  
         i f   ( s t r l e n ( n o r m a l i z e d )   <   2 )   r e t u r n   N U L L ;  
          
         / /   C r e a t e   i n i t i a l   s t a t e  
         A n a l y s i s S t a t e *   c u r r e n t   =   c r e a t e _ s t a t e ( S T A G E _ P R E F I X _ C H E C K ,   n o r m a l i z e d ,   w o r d ) ;  
         i f   ( ! c u r r e n t )   r e t u r n   N U L L ;  
          
         / /   D F A   L o o p  
         / /   U n l i k e   B F S ,   w e   u p d a t e   ' c u r r e n t '   i n   p l a c e   u n t i l   w e   h i t   R o o t   c h e c k  
         b o o l   f i n i s h e d   =   f a l s e ;  
         i n t   i t e r a t i o n s   =   0 ;  
         c o n s t   i n t   M A X _ D F A _ S T E P S   =   1 0 0 ;  
          
         w h i l e   ( ! f i n i s h e d   & &   i t e r a t i o n s   <   M A X _ D F A _ S T E P S )   {  
                 i t e r a t i o n s + + ;  
                  
                 s w i t c h   ( c u r r e n t - > s t a g e )   {  
                         c a s e   S T A G E _ P R E F I X _ C H E C K :   {  
                                 / /   1 .   C h e c k   R e d u p l i c a t i o n   ( G r e e d y )  
                                 / /   I f   w e   f i n d   r e d u p l i c a t i o n ,   a p p l y   i t   a n d   s t a y   i n   P r e f i x   s t a g e  
                                 R e d u p l i c a t i o n L i s t *   r e d u p s   =   d e t e c t _ r e d u p l i c a t i o n ( c u r r e n t - > r e m a i n i n g ,   l a n g u a g e ) ;  
                                 i f   ( r e d u p s   & &   r e d u p s - > c o u n t   >   0 )   {  
                                         / /   P i c k   " b e s t "   r e d u p l i c a t i o n   ( u s u a l l y   F u l l   >   C V C   >   C V )  
                                         / /   T h e   d e t e c t o r   a l r e a d y   o r d e r s   t h e m   b y   s p e c i f i c i t y / l e n g t h   u s u a l l y  
                                         / /   W e ' l l   t a k e   t h e   f i r s t   o n e   ( i n d e x   0 )  
                                         R e d u p l i c a t i o n *   b e s t _ r e d u p   =   & r e d u p s - > p a t t e r n s [ 0 ] ;  
                                          
                                         s a f e _ s t r c p y ( c u r r e n t - > r e m a i n i n g ,   b e s t _ r e d u p - > b a s e ,   s i z e o f ( c u r r e n t - > r e m a i n i n g ) ) ;  
                                         c u r r e n t - > r e d u p _ t y p e   =   b e s t _ r e d u p - > t y p e ;  
                                         s a f e _ s t r c p y ( c u r r e n t - > r e d u p _ r e m o v e d ,   b e s t _ r e d u p - > r e m o v e d ,   s i z e o f ( c u r r e n t - > r e d u p _ r e m o v e d ) ) ;  
                                          
                                         / /   S t a y   i n   P r e f i x   s t a g e  
                                         f r e e _ r e d u p l i c a t i o n _ l i s t ( r e d u p s ) ;  
                                         c o n t i n u e ;   / /   N e x t   i t e r a t i o n  
                                 }  
                                 f r e e _ r e d u p l i c a t i o n _ l i s t ( r e d u p s ) ;  
                                  
                                 / /   2 .   F i n d   B e s t   P r e f i x  
                                 B e s t M a t c h   b e s t _ p r e f i x ;  
                                 i f   ( f i n d _ b e s t _ p r e f i x ( c u r r e n t - > r e m a i n i n g ,   l a n g u a g e ,   & b e s t _ p r e f i x ) )   {  
                                         / /   A p p l y   t r a n s i t i o n  
                                         s a f e _ s t r c p y ( c u r r e n t - > r e m a i n i n g ,   b e s t _ p r e f i x . c a n d i d a t e . f o r m ,   s i z e o f ( c u r r e n t - > r e m a i n i n g ) ) ;  
                                          
                                         i f   ( c u r r e n t - > a f f i x _ c o u n t   <   M A X _ A F F I X E S )   {  
                                                 M a t c h e d A f f i x *   m a   =   & c u r r e n t - > a f f i x e s [ c u r r e n t - > a f f i x _ c o u n t ] ;  
                                                 m a - > a f f i x   =   b e s t _ p r e f i x . a f f i x ;  
                                                 m a - > d e f i n i t i o n   =   b e s t _ p r e f i x . a f f i x - > d e f i n i t i o n s ;  
                                                 s a f e _ s t r c p y ( m a - > m a t c h e d _ f o r m ,   b e s t _ p r e f i x . m a t c h e d _ f o r m ,   s i z e o f ( m a - > m a t c h e d _ f o r m ) ) ;  
                                                 m a - > p o s i t i o n   =   P O S _ P R E F I X ;  
                                                 c u r r e n t - > a f f i x _ c o u n t + + ;  
                                         }  
                                          
                                         i f   ( s t r l e n ( b e s t _ p r e f i x . c a n d i d a t e . c h a n g e s )   >   0 )   {  
                                                 s t r c a t ( c u r r e n t - > m o r p h o _ c h a n g e s ,   b e s t _ p r e f i x . c a n d i d a t e . c h a n g e s ) ;  
                                                 s t r c a t ( c u r r e n t - > m o r p h o _ c h a n g e s ,   " ; " ) ;  
                                         }  
                                          
                                         / /   C h e c k   C i r c u m f i x   S t a r t   ( s i m i l a r   t o   N F A )  
                                         A f f i x I t e r a t o r   c f i x _ i t e r ;  
                                         a f f i x _ i t e r _ i n i t ( & c f i x _ i t e r ,   g _ c i r c u m f i x _ t a b l e ) ;  
                                         A f f i x *   c f i x ;  
                                         w h i l e   ( ( c f i x   =   a f f i x _ i t e r _ n e x t ( & c f i x _ i t e r ) )   ! =   N U L L )   {  
                                                 / /   p r e f i x   m a t c h   c h e c k . . .  
                                                 / /   R e - d e r i v e   p r e f i x   f o r m   f r o m   k e y   ( r e m o v e   h y p h e n )  
                                                 c h a r   p f o r m [ 6 4 ] ;  
                                                 s a f e _ s t r c p y ( p f o r m ,   b e s t _ p r e f i x . m a t c h e d _ f o r m ,   s i z e o f ( p f o r m ) ) ;  
                                                 s i z e _ t   l   =   s t r l e n ( p f o r m ) ;  
                                                 i f   ( l > 0   & &   p f o r m [ l - 1 ] = = ' - ' )   p f o r m [ l - 1 ] = ' \ 0 ' ;  
                                                  
                                                 i f   ( s t a r t s _ w i t h ( c f i x - > p r e f i x _ p a r t ,   p f o r m ) )   {  
                                                         / /   D F A   D e c i s i o n :   I f   w e   m a t c h e d   a   p r e f i x   t h a t   s t a r t s   a   c i r c u m f i x ,  
                                                         / /   w e   a g g r e s s i v e l y   a s s u m e   i t   I S   a   c i r c u m f i x   s t a r t   i f   w e   a r e   i n   g r e e d y   m o d e ?  
                                                         / /   O r   w e   j u s t   r e c o r d   i t .  
                                                         c u r r e n t - > h a s _ c i r c u m f i x _ s t a r t   =   t r u e ;  
                                                         s a f e _ s t r c p y ( c u r r e n t - > e x p e c t e d _ s u f f i x ,   c f i x - > s u f f i x _ p a r t ,   s i z e o f ( c u r r e n t - > e x p e c t e d _ s u f f i x ) ) ;  
                                                         c u r r e n t - > p e n d i n g _ c i r c u m f i x   =   c f i x ;  
                                                         c u r r e n t - > c i r c u m f i x _ p r e f i x _ i n d e x   =   c u r r e n t - > a f f i x _ c o u n t   -   1 ;  
                                                         b r e a k ;   / /   G r e e d y :   t a k e   f i r s t   m a t c h i n g   c i r c u m f i x   d e f i n i t i o n  
                                                 }  
                                         }  
                                          
                                         / /   L o o p   a g a i n   i n   P r e f i x   s t a g e   ( n e s t e d   p r e f i x e s )  
                                         c o n t i n u e ;  
                                 }  
                                  
                                 / /   N o   m o r e   p r e f i x e s   - >   M o v e   t o   I n f i x  
                                 c u r r e n t - > s t a g e   =   S T A G E _ I N F I X _ C H E C K ;  
                                 b r e a k ;  
                         }  
                          
                         c a s e   S T A G E _ I N F I X _ C H E C K :   {  
                                   / /   G r e e d y   I n f i x :   F i n d   f i r s t   v a l i d   i n f i x  
                                   / /   I n   B F S   w e   i t e r a t e d   a l l .   H e r e   v a l i d   c h e c k   i n v o l v e s   i t e r a t i n g   t a b l e .  
                                   / /   W e   r e u s e   t h e   l o g i c   b u t   b r e a k   a f t e r   f i r s t   f i n d ?  
                                   / /   L e t ' s   c o p y - p a s t e   s i m p l i f i e d   l o g i c   f o r   s i n g l e   F i n d .  
                                    
                                   b o o l   m a t c h e d _ i n f i x   =   f a l s e ;  
                                   A f f i x I t e r a t o r   i t e r ;  
                                   a f f i x _ i t e r _ i n i t ( & i t e r ,   g _ i n f i x _ t a b l e ) ;  
                                   A f f i x *   i n f i x ;  
                                   w h i l e   ( ( i n f i x   =   a f f i x _ i t e r _ n e x t ( & i t e r ) )   ! =   N U L L )   {  
                                           i f   ( ! a f f i x _ m a t c h e s _ l a n g u a g e ( i n f i x ,   l a n g u a g e ) )   c o n t i n u e ;  
                                            
                                           c h a r   i n f i x _ f o r m [ 3 2 ] ;  
                                           s a f e _ s t r c p y ( i n f i x _ f o r m ,   i n f i x - > k e y ,   s i z e o f ( i n f i x _ f o r m ) ) ;  
                                           c h a r *   s t a r t   =   i n f i x _ f o r m ;  
                                           i f   ( * s t a r t   = =   ' - ' )   s t a r t + + ;  
                                           s i z e _ t   l e n   =   s t r l e n ( s t a r t ) ;  
                                           i f   ( l e n   >   0   & &   s t a r t [ l e n - 1 ]   = =   ' - ' )   {   s t a r t [ l e n - 1 ] = ' \ 0 ' ;   l e n - - ;   }  
                                            
                                           s i z e _ t   r e m _ l e n   =   s t r l e n ( c u r r e n t - > r e m a i n i n g ) ;  
                                           i f   ( r e m _ l e n   >   l e n   +   1   & &   i s _ c o n s o n a n t ( c u r r e n t - > r e m a i n i n g [ 0 ] ) )   {  
                                                   i f   ( s t r n c m p ( c u r r e n t - > r e m a i n i n g   +   1 ,   s t a r t ,   l e n )   = =   0 )   {  
                                                           / /   F o u n d !  
                                                           c h a r   s t r i p p e d [ 2 5 6 ] ;  
                                                           s t r i p p e d [ 0 ]   =   c u r r e n t - > r e m a i n i n g [ 0 ] ;  
                                                           s t r c p y ( s t r i p p e d   +   1 ,   c u r r e n t - > r e m a i n i n g   +   1   +   l e n ) ;  
                                                           s a f e _ s t r c p y ( c u r r e n t - > r e m a i n i n g ,   s t r i p p e d ,   s i z e o f ( c u r r e n t - > r e m a i n i n g ) ) ;  
                                                            
                                                           i f   ( c u r r e n t - > a f f i x _ c o u n t   <   M A X _ A F F I X E S )   {  
                                                                   M a t c h e d A f f i x *   m a   =   & c u r r e n t - > a f f i x e s [ c u r r e n t - > a f f i x _ c o u n t ] ;  
                                                                   m a - > a f f i x   =   i n f i x ;  
                                                                   m a - > d e f i n i t i o n   =   i n f i x - > d e f i n i t i o n s ;  
                                                                   s a f e _ s t r c p y ( m a - > m a t c h e d _ f o r m ,   i n f i x - > k e y ,   s i z e o f ( m a - > m a t c h e d _ f o r m ) ) ;  
                                                                   m a - > p o s i t i o n   =   P O S _ I N F I X ;  
                                                                   c u r r e n t - > a f f i x _ c o u n t + + ;  
                                                           }  
                                                           m a t c h e d _ i n f i x   =   t r u e ;  
                                                           b r e a k ;   / /   D e t e r m i n i s t i c :   F i r s t   m a t c h   w i n s  
                                                   }  
                                           }  
                                   }  
                                    
                                   / /   M o v e   t o   S u f f i x   s t a g e  
                                   c u r r e n t - > s t a g e   =   S T A G E _ S U F F I X _ C H E C K ;  
                                   b r e a k ;  
                         }  
                          
                         c a s e   S T A G E _ S U F F I X _ C H E C K :   {  
                                 / /   F i n d   B e s t   S u f f i x   ( L o n g e s t )  
                                 B e s t M a t c h   b e s t _ s u f f i x ;  
                                 i f   ( f i n d _ b e s t _ s u f f i x ( c u r r e n t - > r e m a i n i n g ,   l a n g u a g e ,   & b e s t _ s u f f i x ,   c u r r e n t ) )   {  
                                         s a f e _ s t r c p y ( c u r r e n t - > r e m a i n i n g ,   b e s t _ s u f f i x . c a n d i d a t e . f o r m ,   s i z e o f ( c u r r e n t - > r e m a i n i n g ) ) ;  
                                          
                                         / /   H a n d l e   c i r c u m f i x   c o n v e r s i o n  
                                         i f   ( c u r r e n t - > h a s _ c i r c u m f i x _ s t a r t   & &   c u r r e n t - > p e n d i n g _ c i r c u m f i x   & &  
                                                 c u r r e n t - > c i r c u m f i x _ p r e f i x _ i n d e x   > =   0 )   {  
                                                   M a t c h e d A f f i x *   m a   =   & c u r r e n t - > a f f i x e s [ c u r r e n t - > c i r c u m f i x _ p r e f i x _ i n d e x ] ;  
                                                   m a - > a f f i x   =   c u r r e n t - > p e n d i n g _ c i r c u m f i x ;  
                                                   m a - > d e f i n i t i o n   =   c u r r e n t - > p e n d i n g _ c i r c u m f i x - > d e f i n i t i o n s ;  
                                                   s a f e _ s t r c p y ( m a - > m a t c h e d _ f o r m ,   c u r r e n t - > p e n d i n g _ c i r c u m f i x - > k e y ,   s i z e o f ( m a - > m a t c h e d _ f o r m ) ) ;  
                                                   m a - > p o s i t i o n   =   P O S _ C I R C U M F I X ;  
                                                    
                                                   c u r r e n t - > h a s _ c i r c u m f i x _ s t a r t   =   f a l s e ;  
                                                   c u r r e n t - > p e n d i n g _ c i r c u m f i x   =   N U L L ;  
                                         }   e l s e   i f   ( c u r r e n t - > a f f i x _ c o u n t   <   M A X _ A F F I X E S )   {  
                                                   M a t c h e d A f f i x *   m a   =   & c u r r e n t - > a f f i x e s [ c u r r e n t - > a f f i x _ c o u n t ] ;  
                                                   m a - > a f f i x   =   b e s t _ s u f f i x . a f f i x ;  
                                                   m a - > d e f i n i t i o n   =   b e s t _ s u f f i x . a f f i x - > d e f i n i t i o n s ;  
                                                   s a f e _ s t r c p y ( m a - > m a t c h e d _ f o r m ,   b e s t _ s u f f i x . m a t c h e d _ f o r m ,   s i z e o f ( m a - > m a t c h e d _ f o r m ) ) ;  
                                                   m a - > p o s i t i o n   =   P O S _ S U F F I X ;  
                                                   c u r r e n t - > a f f i x _ c o u n t + + ;  
                                         }  
                                          
                                         i f   ( s t r l e n ( b e s t _ s u f f i x . c a n d i d a t e . c h a n g e s )   >   0 )   {  
                                                 s t r c a t ( c u r r e n t - > m o r p h o _ c h a n g e s ,   b e s t _ s u f f i x . c a n d i d a t e . c h a n g e s ) ;  
                                                 s t r c a t ( c u r r e n t - > m o r p h o _ c h a n g e s ,   " ; " ) ;  
                                         }  
                                          
                                         / /   S t a y   i n   S u f f i x   s t a g e   ( n e s t e d   s u f f i x e s )  
                                         c o n t i n u e ;  
                                 }  
                                  
                                 / /   N o   m o r e   s u f f i x e s   - >   R o o t   C h e c k  
                                 c u r r e n t - > s t a g e   =   S T A G E _ R O O T _ C H E C K ;  
                                 f i n i s h e d   =   t r u e ;   / /   E x i t   l o o p   t o   p e r f o r m   f i n a l   c h e c k  
                                 b r e a k ;  
                         }  
                          
                         d e f a u l t :  
                                 f i n i s h e d   =   t r u e ;  
                                 b r e a k ;  
                 }  
         }  
          
         / /   F i n a l   R o o t   C h e c k  
         A n a l y s i s R e s u l t *   r e s u l t s   =   N U L L ;  
         p r o c e s s _ r o o t _ s t a g e ( c u r r e n t ,   & r e s u l t s ,   l a n g u a g e ) ;  
          
         / /   C l e a n   u p   s t a t e  
         f r e e _ s t a t e ( c u r r e n t ) ;  
          
         r e t u r n   r e s u l t s ;   / /   R e t u r n s   t h e   s i n g l e   r e s u l t   ( h e a d   o f   l i s t )  
 }  
 