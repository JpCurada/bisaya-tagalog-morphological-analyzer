#include "analyzer.h"
#include "dictionary.h"
#include "utils.h"
#include "morphology.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cJSON.h"

// Global Hash Tables
static HashTable* prefix_table = NULL;
static HashTable* suffix_table = NULL;
static HashTable* bisaya_roots = NULL;
static HashTable* tagalog_roots = NULL;
static HashTable* hiligaynon_roots = NULL;
static HashTable* shared_vocab = NULL;

// Helper: Populate hash table from raw data string (Pipe-separated)
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

// Helper: Recursively traverse JSON to find affixes
static void process_affix_node(cJSON* node, const char* current_category) {
    if (!node) return;

    // specialized structure: "affixes": ["mu-", "mag-"]
    cJSON* affixes = cJSON_GetObjectItem(node, "affixes");
    if (affixes && cJSON_IsArray(affixes)) {
        cJSON* affix = NULL;
        cJSON_ArrayForEach(affix, affixes) {
            const char* affix_str = NULL;
            if (cJSON_IsString(affix)) {
                affix_str = affix->valuestring;
            } else if (cJSON_IsObject(affix)) {
                cJSON* val = cJSON_GetObjectItem(affix, "affix");
                if (val && cJSON_IsString(val)) {
                    affix_str = val->valuestring;
                }
            }

            if (affix_str) {
                char info[256];
                snprintf(info, 256, "%s", current_category ? current_category : "Unknown"); 
                
                size_t len = strlen(affix_str);
                if (len > 0) {
                     if (affix_str[len-1] == '-') {
                         // Prefix
                         char clean[64];
                         snprintf(clean, sizeof(clean), "%.*s", (int)(len-1), affix_str);
                         ht_insert(prefix_table, clean, info);
                         ht_insert(prefix_table, affix_str, info);
                         printf("DEBUG: Inserted prefix '%s' and '%s'\n", clean, affix_str);
                     } else if (affix_str[0] == '-') {
                         // Suffix
                         char clean[64];
                         strcpy(clean, affix_str + 1);
                         ht_insert(suffix_table, clean, info);
                         ht_insert(suffix_table, affix_str, info);
                     } else {
                         // Infix or circumfix? 
                     }
                }
            }
        }
    }
    
    // Recursive step
    cJSON* child = NULL;
    cJSON_ArrayForEach(child, node) {
        if (cJSON_IsObject(child)) {
             process_affix_node(child, current_category); // Pass down info? 
             // We need to capture the key as category info?
             // Complex traversing logic needed to capture path (e.g. "actor_focus_active")?
             // For now, let's keep it simple.
        }
    }
}

// Better Traversal to capture keys as categories
static void traverse_json_tree(cJSON* item, char* context_buf) {
    if (!item) return;

    if (cJSON_IsObject(item)) {
        // Check if this object is an "affix group" (has 'affixes' array)
        if (cJSON_HasObjectItem(item, "affixes")) {
             // cJSON* affixes = cJSON_GetObjectItem(item, "affixes"); // Unused
             // Extract description/meaning to use as value
             cJSON* desc = cJSON_GetObjectItem(item, "description");
             const char* val_str = desc ? desc->valuestring : context_buf;
             
             process_affix_node(item, val_str);
        } else {
            // Traverse children, append key to context?
            cJSON* child = item->child;
            while (child) {
                // If child has a name (key), append to context? 
                // e.g. "verbal_affixes"
                char new_context[256];
                if (child->string) {
                    snprintf(new_context, 256, "%s", child->string);
                } else {
                     strcpy(new_context, context_buf);
                }
                traverse_json_tree(child, new_context);
                child = child->next;
            }
        }
    }
}

void init_analyzer(const char* affix_json_content,
                   const char* bisaya_data, const char* tagalog_data,
                   const char* hiligaynon_data,
                   const char* shared_data) {
    prefix_table = ht_create(200); // Increased size
    suffix_table = ht_create(200);
    bisaya_roots = ht_create(25000); // Increased for dictionary size
    tagalog_roots = ht_create(25000);
    hiligaynon_roots = ht_create(25000);
    shared_vocab = ht_create(100);

    // Parse JSON
    if (affix_json_content) {
        cJSON* root = cJSON_Parse(affix_json_content);
        if (root) {
            traverse_json_tree(root, "General");
            cJSON_Delete(root);
        } else {
            printf("Error parsing affix JSON.\n");
        }
    }

    populate_table(bisaya_roots, bisaya_data);
    populate_table(tagalog_roots, tagalog_data);
    populate_table(hiligaynon_roots, hiligaynon_data);
    populate_table(shared_vocab, shared_data);
}

void cleanup_analyzer() {
    if (prefix_table) ht_destroy(prefix_table);
    if (suffix_table) ht_destroy(suffix_table);
    if (bisaya_roots) ht_destroy(bisaya_roots);
    if (tagalog_roots) ht_destroy(tagalog_roots);
    if (hiligaynon_roots) ht_destroy(hiligaynon_roots);
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
    
    val = ht_lookup(hiligaynon_roots, word);
    if (val) { *detected_lang = LANG_HILIGAYNON; return val; }

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
    
    // Use greedy matching (longest prefix first) to prefer "nag-" over "nag"
    int max_len = 6;
    if ((int)strlen(current_str) < max_len) max_len = (int)strlen(current_str);
    
    for (int len = max_len; len >= 1; len--) {
        char possible_prefix[16];
        safe_strcpy(possible_prefix, current_str, len + 1); // +1 for null
        // safe_strcpy size is total buffer size. len chars + null needs len+1
        // Wait, safe_strcpy takes size. So to copy 'len' chars, we need buffer of len+1.
        // strncpy is safer here.
        memcpy(possible_prefix, current_str, len);
        possible_prefix[len] = '\0';
        
        prefix_val = ht_lookup(prefix_table, possible_prefix);
        printf("DEBUG: Lookup '%s' (len %d) -> %s\n", possible_prefix, len, prefix_val ? "FOUND" : "NULL");
        if (prefix_val) {
             safe_strcpy(res->prefix, possible_prefix, 64);
             safe_strcpy(res->prefix_info, prefix_val, 256);
             safe_strcpy(stripped_word, current_str + len, MAX_WORD_LEN);
             // ...
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
        
        // Parse root_val "Language|POS|Def"
        char temp_val[MAX_WORD_LEN * 4];
        safe_strcpy(temp_val, root_val, sizeof(temp_val));
        char* token = strtok(temp_val, "|"); // Language
        if (token) {
             token = strtok(NULL, "|"); // POS
             if (token) safe_strcpy(res->root_pos, token, 64);
             token = strtok(NULL, "|"); // Definition
             if (token) safe_strcpy(res->root_definition, token, 1024);
        }

        add_trace(res, "q1->q2: Root identified directly");
    } else {
        add_trace(res, "q1->q1: Root not found, checking suffix");
        // Attempt 2: Try stripping suffix
        size_t slen = strlen(stripped_word);
        char* suffix_val = NULL;
        
        for (int len = 1; len <= 5 && len < (int)slen; len++) {
            char possible_suffix[16];
            const char* start = stripped_word + slen - len;
            safe_strcpy(possible_suffix, start, 16);
            
            suffix_val = ht_lookup(suffix_table, possible_suffix);
            if (suffix_val) {
                char potential_root[MAX_WORD_LEN];
                size_t root_len = slen - len;
                memcpy(potential_root, stripped_word, root_len);
                potential_root[root_len] = '\0';
                
                 root_val = find_root(potential_root, &root_lang);
                 if (root_val) {
                     safe_strcpy(res->root, potential_root, 128);
                     safe_strcpy(res->suffix, possible_suffix, 64);
                     safe_strcpy(res->suffix_info, suffix_val, 256);
                     res->language = root_lang;
                     
                     // Parse root_val
                    char temp_val[MAX_WORD_LEN * 4];
                    safe_strcpy(temp_val, root_val, sizeof(temp_val));
                    char* token = strtok(temp_val, "|"); // Language
                    if (token) {
                         token = strtok(NULL, "|"); // POS
                         if (token) safe_strcpy(res->root_pos, token, 64);
                         token = strtok(NULL, "|"); // Definition
                         if (token) safe_strcpy(res->root_definition, token, 1024);
                    }

                     char msg[64];
                     snprintf(msg, 64, "q1->q2: Suffix '%s' found", possible_suffix);
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
