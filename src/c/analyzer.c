#include "analyzer.h"
#include "dictionary.h"
#include "utils.h"
#include "morphology.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Global Hash Tables
static HashTable* prefix_table = NULL;
static HashTable* suffix_table = NULL;
static HashTable* bisaya_roots = NULL;
static HashTable* tagalog_roots = NULL;
static HashTable* shared_vocab = NULL;

// Helper: Populate hash table from raw data string
static void populate_table(HashTable* table, const char* data) {
    if (!table || !data) return;
    char* data_copy = my_strdup(data);
    char* line = strtok(data_copy, "\n");
    while (line) {
        // Format: key|value
        char* sep = strchr(line, '|');
        if (sep) {
            *sep = '\0';
            char* key = line;
            char* val = sep + 1; // "Language|POS|Func"
            ht_insert(table, key, val);
        }
        line = strtok(NULL, "\n");
    }
    free(data_copy);
}

void init_analyzer(const char* prefix_data, const char* suffix_data,
                   const char* bisaya_data, const char* tagalog_data,
                   const char* shared_data) {
    prefix_table = ht_create(50);
    suffix_table = ht_create(50);
    bisaya_roots = ht_create(250);
    tagalog_roots = ht_create(250);
    shared_vocab = ht_create(100);

    populate_table(prefix_table, prefix_data);
    populate_table(suffix_table, suffix_data);
    populate_table(bisaya_roots, bisaya_data);
    populate_table(tagalog_roots, tagalog_data);
    populate_table(shared_vocab, shared_data);
}

void cleanup_analyzer() {
    if (prefix_table) ht_destroy(prefix_table);
    if (suffix_table) ht_destroy(suffix_table);
    if (bisaya_roots) ht_destroy(bisaya_roots);
    if (tagalog_roots) ht_destroy(tagalog_roots);
    if (shared_vocab) ht_destroy(shared_vocab);
}

static void add_trace(AnalysisResult* res, const char* msg) {
    if (res->trace_count < 10) {
        safe_strcpy(res->trace[res->trace_count], msg, 256);
        res->trace_count++;
    }
}

// Check all root tables
static char* find_root(const char* word, Language* detected_lang) {
    char* val = ht_lookup(bisaya_roots, word);
    if (val) { *detected_lang = LANG_BISAYA; return val; }
    
    val = ht_lookup(tagalog_roots, word);
    if (val) { *detected_lang = LANG_TAGALOG; return val; }
    
    val = ht_lookup(shared_vocab, word);
    if (val) { *detected_lang = LANG_BOTH; return val; }
    
    *detected_lang = LANG_UNKNOWN;
    return NULL;
}

void analyze_word(const char* word, AnalysisResult* res) {
    memset(res, 0, sizeof(AnalysisResult));
    safe_strcpy(res->word, word, MAX_WORD_LEN);
    res->valid = false;
    res->language = LANG_UNKNOWN;

    char current_str[MAX_WORD_LEN];
    safe_strcpy(current_str, word, MAX_WORD_LEN);
    
    // DFA STATE: q0 (Start)
    add_trace(res, "q0: Start analysis");
    
    // TRANSITION: q0 -> q1 (Prefix Stripping)
    // Check for prefix
    // Iterate through prefix table? No, inefficient.
    // Iterating keys in hash table is hard without iterator.
    // Better: We need to check if word starts with any known prefix.
    // Since we don't have list of keys easily, we can iterate specific expected prefixes 
    // OR (better for this simplistic design) we rely on Python sending a list of prefixes 
    // OR we modify HashTable to support iteration.
    // Hack for now: Logic should probably be "try to identify prefix by brute force from table"? 
    // Oh, `prefix_table` lookup requires exact key.
    // We can iterate 1..N chars of word and check if it exists in prefix_table.
    // Yes, max prefix length is small (e.g. 5-6).
    
    char* prefix_val = NULL;
    char stripped_word[MAX_WORD_LEN];
    safe_strcpy(stripped_word, current_str, MAX_WORD_LEN);
    
    for (int len = 1; len <= 6 && len < (int)strlen(current_str); len++) {
        char possible_prefix[16];
        safe_strcpy(possible_prefix, current_str, len + 1); // +1 for null
        // safe_strcpy size is total buffer size. len chars + null needs len+1
        // Wait, safe_strcpy takes size. So to copy 'len' chars, we need buffer of len+1.
        // strncpy is safer here.
        strncpy(possible_prefix, current_str, len);
        possible_prefix[len] = '\0';
        
        prefix_val = ht_lookup(prefix_table, possible_prefix);
        if (prefix_val) {
            safe_strcpy(res->prefix, possible_prefix, 64);
            safe_strcpy(stripped_word, current_str + len, MAX_WORD_LEN);
            // If prefix ends with '-', it's usually stripped in lexicon but present in word? 
            // Our lexicon keys include '-'? 
            // data/prefix_table.json has "nag-". 
            // So if word is "nagluto", possible_prefix "nag" won't match "nag-".
            // We need to handle the hyphen.
            // If user input is "nag-luto", then "nag-" matches.
            // If user input is "nagluto", then "nag" won't match "nag-".
            // This is a complexity.
            // The proposal examples: "mag-luto", "nag-kaon".
            // Implementation plan: "Strip prefix... word starts with...".
            // Let's assume input includes hyphens as per examples? 
            // Or lexicon keys should be bare "nag"?
            // Plan says keys are "nag-".
            // Let's assume input text uses hyphens for now as per "nag-kaon" example.
            // If not, we might need to try adding '-' to lookup.
            
            // Let's try adding '-' if not present in check.
            // Actually, let's assume strict match first.
            char msg[64];
            snprintf(msg, 64, "q0->q1: Found prefix '%s'", possible_prefix);
            add_trace(res, msg);
            break;
        }
    }
    
    if (!prefix_val) {
        add_trace(res, "q0->q1: No prefix found");
    }

    // STATE: q1 (Prefix Stripped)
    // TRANSITION: q1 -> q2 (Root Identification)
    char* root_val = NULL;
    Language root_lang = LANG_UNKNOWN;
    
    // Attempt 1: Remaining is root
    root_val = find_root(stripped_word, &root_lang);
    
    if (root_val) {
        safe_strcpy(res->root, stripped_word, 128);
        res->language = root_lang;
        add_trace(res, "q1->q2: Root identified directly");
    } else {
        add_trace(res, "q1->q1: Root not found, checking suffix");
        // Attempt 2: Try stripping suffix
        // Same iteration strategy (from end)
        size_t slen = strlen(stripped_word);
        char* suffix_val = NULL;
        
        for (int len = 1; len <= 5 && len < (int)slen; len++) {
            char possible_suffix[16];
            const char* start = stripped_word + slen - len;
            safe_strcpy(possible_suffix, start, 16);
             // Again, handle hyphen? "kaon-an". Suffix "-an".
             // If input is "kaonan", suffix "an" != "-an".
             // Assuming hyphenated input for now: "kaon-an".
            
            suffix_val = ht_lookup(suffix_table, possible_suffix);
            if (suffix_val) {
                // Potential suffix found
                // Check if remainder is root
                char potential_root[MAX_WORD_LEN];
                size_t root_len = slen - len;
                strncpy(potential_root, stripped_word, root_len);
                potential_root[root_len] = '\0';
                
                // If hyphenated "kaon-an", suffix is "-an", len is 3. 
                // "kaon" is root. 
                // If suffix check includes hyphen, we need to be careful.
                
                 root_val = find_root(potential_root, &root_lang);
                 if (root_val) {
                     safe_strcpy(res->root, potential_root, 128);
                     safe_strcpy(res->suffix, possible_suffix, 64);
                     res->language = root_lang;
                     char msg[64];
                     snprintf(msg, 64, "q1->q2: Suffix '%s' found, Root identified", possible_suffix);
                     add_trace(res, msg);
                     break;
                 }
            }
        }
    }
    
    if (!root_val) {
        add_trace(res, "q1->REJECT: No valid root found");
        res->valid = false;
        return;
    }

    // STATE: q2 (Root Found) -> q3 (Suffix Checked) based on logic above
    // Actually our logic combined root+suffix search.
    // If we are here, we have root, and optionally suffix.
    add_trace(res, "q2->q3: Structure parsed");
    
    // STATE: q3 -> q4 (Validation)
    char* suffix_lookup_val = NULL;
    if (res->suffix[0] != '\0') {
         suffix_lookup_val = ht_lookup(suffix_table, res->suffix);
    }
    
    // Re-lookup prefix value for validation (we just stored text before)
    if (res->prefix[0] != '\0' && !prefix_val) {
         prefix_val = ht_lookup(prefix_table, res->prefix);
    }
    
    bool compatible = validate_morphology(prefix_val, root_val, suffix_lookup_val);
    
    if (compatible) {
        res->valid = true;
        add_trace(res, "q3->q4: ACCEPT - Valid morphology");
    } else {
        res->valid = false;
        add_trace(res, "q3->REJECT: Incompatible morphology");
    }
}
