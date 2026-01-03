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
extern HashTable* shared_vocab;

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
        
        // Check shared vocabulary
        if (!root_val && shared_vocab) {
            root_val = ht_lookup(shared_vocab, remaining);
            if (root_val) {
                root_gloss = root_val;
                detected_lang = LANG_BOTH;
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
HashTable* shared_vocab = NULL;

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
    shared_vocab = ht_create(100);

    populate_table(legacy_prefix_table, prefix_data);
    populate_table(legacy_suffix_table, suffix_data);
    populate_table(bisaya_roots, bisaya_data);
    populate_table(tagalog_roots, tagalog_data);
    populate_table(shared_vocab, shared_data);
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
    if (shared_vocab) ht_destroy(shared_vocab);
    
    legacy_prefix_table = NULL;
    legacy_suffix_table = NULL;
    bisaya_roots = NULL;
    tagalog_roots = NULL;
    shared_vocab = NULL;
    
    cleanup_analyzer_v2();
}