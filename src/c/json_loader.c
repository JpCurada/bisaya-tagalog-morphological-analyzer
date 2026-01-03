#include "json_loader.h"
#include "cJSON.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global affix tables
AffixTable* g_prefix_table = NULL;
AffixTable* g_suffix_table = NULL;
AffixTable* g_infix_table = NULL;
AffixTable* g_circumfix_table = NULL;

// DJB2 hash function
static unsigned long hash_affix_key(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// Create an affix table
static AffixTable* affix_table_create(unsigned int size, AffixPosition pos_type) {
    AffixTable* table = (AffixTable*)malloc(sizeof(AffixTable));
    if (!table) return NULL;
    
    table->buckets = (Affix**)calloc(size, sizeof(Affix*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }
    
    table->size = size;
    table->count = 0;
    table->position_type = pos_type;
    
    return table;
}

// Read entire file into string
static char* read_file_contents(const char* filepath) {
    FILE* fp = fopen(filepath, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s\n", filepath);
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* content = (char*)malloc(size + 1);
    if (!content) {
        fclose(fp);
        return NULL;
    }
    
    size_t read_size = fread(content, 1, size, fp);
    content[read_size] = '\0';
    
    fclose(fp);
    return content;
}

// Parse a single AffixDef from cJSON object
static AffixDef* parse_affix_def(cJSON* json_def) {
    if (!json_def) return NULL;
    
    AffixDef* def = (AffixDef*)calloc(1, sizeof(AffixDef));
    if (!def) return NULL;
    
    // Parse each field
    cJSON* item;
    
    item = cJSON_GetObjectItem(json_def, "type");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->type, item->valuestring, sizeof(def->type));
    }
    
    item = cJSON_GetObjectItem(json_def, "language");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->language, item->valuestring, sizeof(def->language));
    }
    
    item = cJSON_GetObjectItem(json_def, "focus");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->focus, item->valuestring, sizeof(def->focus));
    }
    
    item = cJSON_GetObjectItem(json_def, "aspect");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->aspect, item->valuestring, sizeof(def->aspect));
    }
    
    item = cJSON_GetObjectItem(json_def, "tense");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->tense, item->valuestring, sizeof(def->tense));
    }
    
    item = cJSON_GetObjectItem(json_def, "function");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->function, item->valuestring, sizeof(def->function));
    }
    
    item = cJSON_GetObjectItem(json_def, "rules");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->rules, item->valuestring, sizeof(def->rules));
    }
    
    item = cJSON_GetObjectItem(json_def, "allomorphs");
    if (item && cJSON_IsString(item)) {
        safe_strcpy(def->allomorphs, item->valuestring, sizeof(def->allomorphs));
    }
    
    // Parse example object
    cJSON* example = cJSON_GetObjectItem(json_def, "example");
    if (example && cJSON_IsObject(example)) {
        item = cJSON_GetObjectItem(example, "root_ceb");
        if (item && cJSON_IsString(item)) {
            safe_strcpy(def->example_root_ceb, item->valuestring, sizeof(def->example_root_ceb));
        }
        
        item = cJSON_GetObjectItem(example, "form_ceb");
        if (item && cJSON_IsString(item)) {
            safe_strcpy(def->example_form_ceb, item->valuestring, sizeof(def->example_form_ceb));
        }
        
        item = cJSON_GetObjectItem(example, "gloss_ceb");
        if (item && cJSON_IsString(item)) {
            safe_strcpy(def->example_gloss_ceb, item->valuestring, sizeof(def->example_gloss_ceb));
        }
        
        item = cJSON_GetObjectItem(example, "root_tag");
        if (item && cJSON_IsString(item)) {
            safe_strcpy(def->example_root_tag, item->valuestring, sizeof(def->example_root_tag));
        }
        
        item = cJSON_GetObjectItem(example, "form_tag");
        if (item && cJSON_IsString(item)) {
            safe_strcpy(def->example_form_tag, item->valuestring, sizeof(def->example_form_tag));
        }
        
        item = cJSON_GetObjectItem(example, "gloss_tag");
        if (item && cJSON_IsString(item)) {
            safe_strcpy(def->example_gloss_tag, item->valuestring, sizeof(def->example_gloss_tag));
        }
    }
    
    def->next = NULL;
    return def;
}

// Parse circumfix key into prefix and suffix parts
// e.g., "gi-...-an" -> prefix_part="gi-", suffix_part="-an"
static void parse_circumfix_parts(const char* key, char* prefix_part, char* suffix_part, 
                                   size_t prefix_size, size_t suffix_size) {
    prefix_part[0] = '\0';
    suffix_part[0] = '\0';
    
    // Find "..." separator
    const char* sep = strstr(key, "...");
    if (!sep) {
        // Not a valid circumfix pattern
        return;
    }
    
    // Copy prefix part (up to and including the separator)
    size_t prefix_len = sep - key;
    if (prefix_len > 0 && prefix_len < prefix_size) {
        memcpy(prefix_part, key, prefix_len);
        prefix_part[prefix_len] = '\0';
    }
    
    // Copy suffix part (after "...")
    const char* suffix_start = sep + 3;  // Skip "..."
    if (*suffix_start) {
        safe_strcpy(suffix_part, suffix_start, suffix_size);
    }
}

// Insert affix into table
static void affix_table_insert(AffixTable* table, Affix* affix) {
    if (!table || !affix) return;
    
    unsigned long index = hash_affix_key(affix->key) % table->size;
    
    // Check if key already exists
    Affix* current = table->buckets[index];
    while (current) {
        if (strcmp(current->key, affix->key) == 0) {
            // Key exists, append definitions
            AffixDef* tail = current->definitions;
            while (tail->next) {
                tail = tail->next;
            }
            tail->next = affix->definitions;
            current->def_count += affix->def_count;
            free(affix);  // Free the duplicate affix struct
            return;
        }
        current = current->next;
    }
    
    // Insert new affix
    affix->next = table->buckets[index];
    table->buckets[index] = affix;
    table->count++;
}

// Lookup affix in table
static Affix* affix_table_lookup(AffixTable* table, const char* key) {
    if (!table || !key) return NULL;
    
    unsigned long index = hash_affix_key(key) % table->size;
    
    Affix* current = table->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Load a JSON affix table
static bool load_affix_table_from_json(const char* filepath, AffixTable* table, AffixPosition pos_type) {
    char* content = read_file_contents(filepath);
    if (!content) return false;
    
    cJSON* json = cJSON_Parse(content);
    free(content);
    
    if (!json) {
        fprintf(stderr, "Error: Failed to parse JSON from %s\n", filepath);
        return false;
    }
    
    // Iterate over all keys in the JSON object
    cJSON* child = json->child;
    while (child) {
        if (cJSON_IsArray(child)) {
            Affix* affix = (Affix*)calloc(1, sizeof(Affix));
            if (!affix) {
                child = child->next;
                continue;
            }
            
            safe_strcpy(affix->key, child->string, sizeof(affix->key));
            affix->position = pos_type;
            affix->def_count = 0;
            affix->definitions = NULL;
            affix->next = NULL;
            
            // Parse circumfix parts if applicable
            if (pos_type == POS_CIRCUMFIX) {
                parse_circumfix_parts(affix->key, affix->prefix_part, affix->suffix_part,
                                     sizeof(affix->prefix_part), sizeof(affix->suffix_part));
            }
            
            // Parse all definitions in the array
            cJSON* def_json;
            AffixDef* last_def = NULL;
            
            cJSON_ArrayForEach(def_json, child) {
                AffixDef* def = parse_affix_def(def_json);
                if (def) {
                    if (!affix->definitions) {
                        affix->definitions = def;
                    } else {
                        last_def->next = def;
                    }
                    last_def = def;
                    affix->def_count++;
                }
            }
            
            affix_table_insert(table, affix);
        }
        
        child = child->next;
    }
    
    cJSON_Delete(json);
    return true;
}

// Public API implementations

bool load_prefix_table(const char* filepath) {
    if (g_prefix_table) {
        // Already loaded
        return true;
    }
    
    g_prefix_table = affix_table_create(128, POS_PREFIX);
    if (!g_prefix_table) return false;
    
    return load_affix_table_from_json(filepath, g_prefix_table, POS_PREFIX);
}

bool load_suffix_table(const char* filepath) {
    if (g_suffix_table) {
        return true;
    }
    
    g_suffix_table = affix_table_create(32, POS_SUFFIX);
    if (!g_suffix_table) return false;
    
    return load_affix_table_from_json(filepath, g_suffix_table, POS_SUFFIX);
}

bool load_infix_table(const char* filepath) {
    if (g_infix_table) {
        return true;
    }
    
    g_infix_table = affix_table_create(16, POS_INFIX);
    if (!g_infix_table) return false;
    
    return load_affix_table_from_json(filepath, g_infix_table, POS_INFIX);
}

bool load_circumfix_table(const char* filepath) {
    if (g_circumfix_table) {
        return true;
    }
    
    g_circumfix_table = affix_table_create(64, POS_CIRCUMFIX);
    if (!g_circumfix_table) return false;
    
    return load_affix_table_from_json(filepath, g_circumfix_table, POS_CIRCUMFIX);
}

bool load_affix_tables(const char* data_dir) {
    char filepath[512];
    bool success = true;
    
    // Load prefix table
    snprintf(filepath, sizeof(filepath), "%s/prefix_table.json", data_dir);
    if (!load_prefix_table(filepath)) {
        fprintf(stderr, "Warning: Failed to load prefix table\n");
        success = false;
    }
    
    // Load suffix table
    snprintf(filepath, sizeof(filepath), "%s/suffix_table.json", data_dir);
    if (!load_suffix_table(filepath)) {
        fprintf(stderr, "Warning: Failed to load suffix table\n");
        success = false;
    }
    
    // Load infix table
    snprintf(filepath, sizeof(filepath), "%s/infix_table.json", data_dir);
    if (!load_infix_table(filepath)) {
        fprintf(stderr, "Warning: Failed to load infix table\n");
        success = false;
    }
    
    // Load circumfix table
    snprintf(filepath, sizeof(filepath), "%s/circumfix_table.json", data_dir);
    if (!load_circumfix_table(filepath)) {
        fprintf(stderr, "Warning: Failed to load circumfix table\n");
        success = false;
    }
    
    return success;
}

Affix* lookup_prefix(const char* key) {
    return affix_table_lookup(g_prefix_table, key);
}

Affix* lookup_suffix(const char* key) {
    return affix_table_lookup(g_suffix_table, key);
}

Affix* lookup_infix(const char* key) {
    return affix_table_lookup(g_infix_table, key);
}

Affix* lookup_circumfix(const char* key) {
    return affix_table_lookup(g_circumfix_table, key);
}

// Helper to collect all affixes from a table into a linked list
// Note: This function was unused and removed to fix compilation warnings
/*
static Affix* collect_all_affixes(AffixTable* table) {
    if (!table) return NULL;
    
    Affix* head = NULL;
    Affix* tail = NULL;
    
    for (unsigned int i = 0; i < table->size; i++) {
        Affix* bucket = table->buckets[i];
        while (bucket) {
            if (!head) {
                head = bucket;
                tail = bucket;
            } else {
                // Note: We're not modifying the table structure
                // Just traversing. The 'next' pointer is already set.
                tail = bucket;
            }
            bucket = bucket->next;
        }
    }
    
    return head;
}
*/

Affix* get_all_prefixes(void) {
    if (!g_prefix_table) return NULL;
    
    // Return first non-null bucket entry
    for (unsigned int i = 0; i < g_prefix_table->size; i++) {
        if (g_prefix_table->buckets[i]) {
            return g_prefix_table->buckets[i];
        }
    }
    return NULL;
}

Affix* get_all_suffixes(void) {
    if (!g_suffix_table) return NULL;
    
    for (unsigned int i = 0; i < g_suffix_table->size; i++) {
        if (g_suffix_table->buckets[i]) {
            return g_suffix_table->buckets[i];
        }
    }
    return NULL;
}

Affix* get_all_infixes(void) {
    if (!g_infix_table) return NULL;
    
    for (unsigned int i = 0; i < g_infix_table->size; i++) {
        if (g_infix_table->buckets[i]) {
            return g_infix_table->buckets[i];
        }
    }
    return NULL;
}

Affix* get_all_circumfixes(void) {
    if (!g_circumfix_table) return NULL;
    
    for (unsigned int i = 0; i < g_circumfix_table->size; i++) {
        if (g_circumfix_table->buckets[i]) {
            return g_circumfix_table->buckets[i];
        }
    }
    return NULL;
}

bool affixdef_matches_language(const AffixDef* def, const char* language) {
    if (!def || !language) return false;
    
    // "Both" matches everything
    if (strcmp(def->language, "Both") == 0) return true;
    
    // Direct match
    if (strcmp(def->language, language) == 0) return true;
    
    return false;
}

bool affix_matches_language(const Affix* affix, const char* language) {
    if (!affix || !language) return false;
    
    // Check if any definition matches
    AffixDef* def = affix->definitions;
    while (def) {
        if (affixdef_matches_language(def, language)) {
            return true;
        }
        def = def->next;
    }
    
    return false;
}

// Free a linked list of AffixDefs
static void free_affix_defs(AffixDef* def) {
    while (def) {
        AffixDef* next = def->next;
        free(def);
        def = next;
    }
}

// Free an affix table
static void free_affix_table(AffixTable* table) {
    if (!table) return;
    
    for (unsigned int i = 0; i < table->size; i++) {
        Affix* affix = table->buckets[i];
        while (affix) {
            Affix* next = affix->next;
            free_affix_defs(affix->definitions);
            free(affix);
            affix = next;
        }
    }
    
    free(table->buckets);
    free(table);
}

void cleanup_affix_tables(void) {
    free_affix_table(g_prefix_table);
    g_prefix_table = NULL;
    
    free_affix_table(g_suffix_table);
    g_suffix_table = NULL;
    
    free_affix_table(g_infix_table);
    g_infix_table = NULL;
    
    free_affix_table(g_circumfix_table);
    g_circumfix_table = NULL;
}

void print_affix_stats(void) {
    printf("=== Affix Table Statistics ===\n");
    
    if (g_prefix_table) {
        printf("Prefixes: %u entries\n", g_prefix_table->count);
    }
    
    if (g_suffix_table) {
        printf("Suffixes: %u entries\n", g_suffix_table->count);
    }
    
    if (g_infix_table) {
        printf("Infixes: %u entries\n", g_infix_table->count);
    }
    
    if (g_circumfix_table) {
        printf("Circumfixes: %u entries\n", g_circumfix_table->count);
    }
    
    printf("==============================\n");
}

// =====================================================
// Root Dictionary Implementation
// =====================================================

RootDictionary* g_bisaya_roots = NULL;
RootDictionary* g_tagalog_roots = NULL;
RootDictionary* g_shared_vocab = NULL;

// Create a root dictionary
static RootDictionary* root_dict_create(unsigned int size, const char* default_lang) {
    RootDictionary* dict = (RootDictionary*)malloc(sizeof(RootDictionary));
    if (!dict) return NULL;
    
    dict->buckets = (RootWord**)calloc(size, sizeof(RootWord*));
    if (!dict->buckets) {
        free(dict);
        return NULL;
    }
    
    dict->size = size;
    dict->count = 0;
    safe_strcpy(dict->language, default_lang, sizeof(dict->language));
    
    return dict;
}

// Insert a root word into dictionary
static void root_dict_insert(RootDictionary* dict, RootWord* root) {
    if (!dict || !root) return;
    
    // Lowercase the word for lookup
    char lower_word[MAX_ROOT_WORD_LEN];
    safe_strcpy(lower_word, root->word, sizeof(lower_word));
    to_lowercase(lower_word);
    
    unsigned long index = hash_affix_key(lower_word) % dict->size;
    
    // Check if key already exists (update or skip)
    RootWord* current = dict->buckets[index];
    while (current) {
        char current_lower[MAX_ROOT_WORD_LEN];
        safe_strcpy(current_lower, current->word, sizeof(current_lower));
        to_lowercase(current_lower);
        
        if (strcmp(current_lower, lower_word) == 0) {
            // Word already exists, skip (keep first definition)
            free(root);
            return;
        }
        current = current->next;
    }
    
    // Insert new entry
    root->next = dict->buckets[index];
    dict->buckets[index] = root;
    dict->count++;
}

// Lookup a root word
static RootWord* root_dict_lookup(RootDictionary* dict, const char* word) {
    if (!dict || !word) return NULL;
    
    char lower_word[MAX_ROOT_WORD_LEN];
    safe_strcpy(lower_word, word, sizeof(lower_word));
    to_lowercase(lower_word);
    
    unsigned long index = hash_affix_key(lower_word) % dict->size;
    
    RootWord* current = dict->buckets[index];
    while (current) {
        char current_lower[MAX_ROOT_WORD_LEN];
        safe_strcpy(current_lower, current->word, sizeof(current_lower));
        to_lowercase(current_lower);
        
        if (strcmp(current_lower, lower_word) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Load root dictionary from array JSON (new format)
static bool load_root_array_json(const char* filepath, RootDictionary* dict) {
    char* content = read_file_contents(filepath);
    if (!content) return false;
    
    cJSON* json = cJSON_Parse(content);
    free(content);
    
    if (!json) {
        fprintf(stderr, "Error: Failed to parse JSON from %s\n", filepath);
        return false;
    }
    
    // Expect an array
    if (!cJSON_IsArray(json)) {
        fprintf(stderr, "Error: Expected array in %s\n", filepath);
        cJSON_Delete(json);
        return false;
    }
    
    // Iterate through array items
    cJSON* item;
    cJSON_ArrayForEach(item, json) {
        if (!cJSON_IsObject(item)) continue;
        
        cJSON* word_item = cJSON_GetObjectItem(item, "word");
        if (!word_item || !cJSON_IsString(word_item)) continue;
        
        RootWord* root = (RootWord*)calloc(1, sizeof(RootWord));
        if (!root) continue;
        
        safe_strcpy(root->word, word_item->valuestring, sizeof(root->word));
        
        cJSON* def_item = cJSON_GetObjectItem(item, "definition");
        if (def_item && cJSON_IsString(def_item)) {
            // Truncate if too long
            size_t def_len = strlen(def_item->valuestring);
            if (def_len < sizeof(root->definition)) {
                safe_strcpy(root->definition, def_item->valuestring, sizeof(root->definition));
            } else {
                strncpy(root->definition, def_item->valuestring, sizeof(root->definition) - 4);
                root->definition[sizeof(root->definition) - 4] = '\0';
                strcat(root->definition, "...");
            }
        }
        
        cJSON* lang_item = cJSON_GetObjectItem(item, "language");
        if (lang_item && cJSON_IsString(lang_item)) {
            safe_strcpy(root->language, lang_item->valuestring, sizeof(root->language));
        } else {
            safe_strcpy(root->language, dict->language, sizeof(root->language));
        }
        
        cJSON* pos_item = cJSON_GetObjectItem(item, "part_of_speech");
        if (pos_item && cJSON_IsString(pos_item)) {
            safe_strcpy(root->pos, pos_item->valuestring, sizeof(root->pos));
        }
        
        cJSON* link_item = cJSON_GetObjectItem(item, "link");
        if (link_item && cJSON_IsString(link_item)) {
            safe_strcpy(root->link, link_item->valuestring, sizeof(root->link));
        }
        
        root_dict_insert(dict, root);
    }
    
    cJSON_Delete(json);
    return true;
}

// Load shared vocab from object JSON (original format)
static bool load_shared_vocab_object_json(const char* filepath, RootDictionary* dict) {
    char* content = read_file_contents(filepath);
    if (!content) return false;
    
    cJSON* json = cJSON_Parse(content);
    free(content);
    
    if (!json) {
        fprintf(stderr, "Error: Failed to parse JSON from %s\n", filepath);
        return false;
    }
    
    // Iterate through object keys
    cJSON* child = json->child;
    while (child) {
        if (cJSON_IsObject(child) && child->string) {
            RootWord* root = (RootWord*)calloc(1, sizeof(RootWord));
            if (root) {
                safe_strcpy(root->word, child->string, sizeof(root->word));
                
                cJSON* meaning = cJSON_GetObjectItem(child, "meaning");
                if (meaning && cJSON_IsString(meaning)) {
                    safe_strcpy(root->definition, meaning->valuestring, sizeof(root->definition));
                }
                
                cJSON* pos = cJSON_GetObjectItem(child, "pos");
                if (pos && cJSON_IsString(pos)) {
                    safe_strcpy(root->pos, pos->valuestring, sizeof(root->pos));
                }
                
                cJSON* origin = cJSON_GetObjectItem(child, "origin");
                if (origin && cJSON_IsString(origin)) {
                    safe_strcpy(root->language, origin->valuestring, sizeof(root->language));
                } else {
                    safe_strcpy(root->language, "Both", sizeof(root->language));
                }
                
                root_dict_insert(dict, root);
            }
        }
        child = child->next;
    }
    
    cJSON_Delete(json);
    return true;
}

// Public API for root loading

bool load_bisaya_roots_json(const char* filepath) {
    if (g_bisaya_roots) return true;  // Already loaded
    
    // Use large hash table for performance with many entries
    g_bisaya_roots = root_dict_create(50000, "Bisaya");
    if (!g_bisaya_roots) return false;
    
    return load_root_array_json(filepath, g_bisaya_roots);
}

bool load_tagalog_roots_json(const char* filepath) {
    if (g_tagalog_roots) return true;
    
    g_tagalog_roots = root_dict_create(20000, "Tagalog");
    if (!g_tagalog_roots) return false;
    
    return load_root_array_json(filepath, g_tagalog_roots);
}

bool load_shared_vocab_json(const char* filepath) {
    if (g_shared_vocab) return true;
    
    g_shared_vocab = root_dict_create(500, "Both");
    if (!g_shared_vocab) return false;
    
    return load_shared_vocab_object_json(filepath, g_shared_vocab);
}

bool load_root_dictionaries(const char* data_dir) {
    char filepath[512];
    bool success = true;
    
    snprintf(filepath, sizeof(filepath), "%s/bisaya_roots.json", data_dir);
    if (!load_bisaya_roots_json(filepath)) {
        fprintf(stderr, "Warning: Failed to load Bisaya roots\n");
        success = false;
    }
    
    snprintf(filepath, sizeof(filepath), "%s/tagalog_roots.json", data_dir);
    if (!load_tagalog_roots_json(filepath)) {
        fprintf(stderr, "Warning: Failed to load Tagalog roots\n");
        success = false;
    }
    
    snprintf(filepath, sizeof(filepath), "%s/shared_vocab.json", data_dir);
    if (!load_shared_vocab_json(filepath)) {
        fprintf(stderr, "Warning: Failed to load shared vocabulary\n");
        success = false;
    }
    
    return success;
}

RootWord* lookup_bisaya_root(const char* word) {
    return root_dict_lookup(g_bisaya_roots, word);
}

RootWord* lookup_tagalog_root(const char* word) {
    return root_dict_lookup(g_tagalog_roots, word);
}

RootWord* lookup_shared_vocab(const char* word) {
    return root_dict_lookup(g_shared_vocab, word);
}

RootWord* lookup_any_root(const char* word, const char* language) {
    RootWord* root = NULL;
    
    // Try based on language preference
    if (!language || strcmp(language, "Bisaya") == 0 || strcmp(language, "Cebuano") == 0) {
        root = lookup_bisaya_root(word);
        if (root) return root;
    }
    
    if (!language || strcmp(language, "Tagalog") == 0) {
        root = lookup_tagalog_root(word);
        if (root) return root;
    }
    
    // Try shared vocabulary
    root = lookup_shared_vocab(word);
    if (root) return root;
    
    // Last resort: try all dictionaries
    if (language) {
        root = lookup_bisaya_root(word);
        if (root) return root;
        
        root = lookup_tagalog_root(word);
        if (root) return root;
    }
    
    return NULL;
}

unsigned int get_bisaya_root_count(void) {
    return g_bisaya_roots ? g_bisaya_roots->count : 0;
}

unsigned int get_tagalog_root_count(void) {
    return g_tagalog_roots ? g_tagalog_roots->count : 0;
}

unsigned int get_shared_vocab_count(void) {
    return g_shared_vocab ? g_shared_vocab->count : 0;
}

// Free a root dictionary
static void free_root_dict(RootDictionary* dict) {
    if (!dict) return;
    
    for (unsigned int i = 0; i < dict->size; i++) {
        RootWord* root = dict->buckets[i];
        while (root) {
            RootWord* next = root->next;
            free(root);
            root = next;
        }
    }
    
    free(dict->buckets);
    free(dict);
}

void cleanup_root_dictionaries(void) {
    free_root_dict(g_bisaya_roots);
    g_bisaya_roots = NULL;
    
    free_root_dict(g_tagalog_roots);
    g_tagalog_roots = NULL;
    
    free_root_dict(g_shared_vocab);
    g_shared_vocab = NULL;
}

void print_root_stats(void) {
    printf("=== Root Dictionary Statistics ===\n");
    
    if (g_bisaya_roots) {
        printf("Bisaya roots: %u entries\n", g_bisaya_roots->count);
    }
    
    if (g_tagalog_roots) {
        printf("Tagalog roots: %u entries\n", g_tagalog_roots->count);
    }
    
    if (g_shared_vocab) {
        printf("Shared vocabulary: %u entries\n", g_shared_vocab->count);
    }
    
    printf("==================================\n");
}
