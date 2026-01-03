#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include <stdbool.h>

/**
 * JSON Loader for affix tables.
 * Loads prefix_table.json, suffix_table.json, infix_table.json, circumfix_table.json
 */

#define MAX_AFFIX_KEY_LEN 64
#define MAX_AFFIX_FIELD_LEN 256
#define MAX_ALLOMORPHS 16

// Affix position types
typedef enum {
    POS_PREFIX,
    POS_SUFFIX,
    POS_INFIX,
    POS_CIRCUMFIX
} AffixPosition;

// Single affix definition (one meaning of an affix)
typedef struct AffixDef {
    char type[32];              // "Inflectional", "Derivational", "Nominal", "Adjectival"
    char language[16];          // "Cebuano", "Tagalog", "Both"
    char focus[64];             // "Actor", "Object", "Locative", "Instrumental", etc.
    char aspect[32];            // "Punctual", "Durative", "Potential", "Completed", etc.
    char tense[32];             // "Future", "Past", "Subjunctive"
    char function[MAX_AFFIX_FIELD_LEN];  // Detailed meaning/function
    char rules[MAX_AFFIX_FIELD_LEN];     // Special constraints and usage notes
    char allomorphs[MAX_AFFIX_FIELD_LEN]; // Related allomorphs info
    
    // Example data
    char example_root_ceb[64];
    char example_form_ceb[64];
    char example_gloss_ceb[128];
    char example_root_tag[64];
    char example_form_tag[64];
    char example_gloss_tag[128];
    
    struct AffixDef* next;      // For multiple definitions with same key
} AffixDef;

// Affix entry (key + all its definitions)
typedef struct Affix {
    char key[MAX_AFFIX_KEY_LEN];    // e.g., "mu-", "gi-...-an"
    AffixPosition position;          // PREFIX, SUFFIX, INFIX, CIRCUMFIX
    
    // For circumfixes, split into parts
    char prefix_part[32];           // e.g., "gi-" from "gi-...-an"
    char suffix_part[32];           // e.g., "-an" from "gi-...-an"
    
    AffixDef* definitions;          // Linked list of definitions (for ambiguity)
    int def_count;                  // Number of definitions
    
    struct Affix* next;             // For hash table chaining
} Affix;

// Affix table (hash table)
typedef struct AffixTable {
    Affix** buckets;
    unsigned int size;
    unsigned int count;
    AffixPosition position_type;    // What type of affixes this table holds
} AffixTable;

// Global affix tables
extern AffixTable* g_prefix_table;
extern AffixTable* g_suffix_table;
extern AffixTable* g_infix_table;
extern AffixTable* g_circumfix_table;

// Initialize all affix tables from JSON files
// data_dir should be the path to the data directory containing the JSON files
bool load_affix_tables(const char* data_dir);

// Load individual tables
bool load_prefix_table(const char* filepath);
bool load_suffix_table(const char* filepath);
bool load_infix_table(const char* filepath);
bool load_circumfix_table(const char* filepath);

// Lookup functions
Affix* lookup_prefix(const char* key);
Affix* lookup_suffix(const char* key);
Affix* lookup_infix(const char* key);
Affix* lookup_circumfix(const char* key);

// Get all affixes from a table (returns linked list)
Affix* get_all_prefixes(void);
Affix* get_all_suffixes(void);
Affix* get_all_infixes(void);
Affix* get_all_circumfixes(void);

// Check if affix matches language
bool affix_matches_language(const Affix* affix, const char* language);
bool affixdef_matches_language(const AffixDef* def, const char* language);

// Cleanup
void cleanup_affix_tables(void);

// Debug/Info
void print_affix_stats(void);

// =====================================================
// Root Word Loading (New JSON format: array of objects)
// =====================================================

#define MAX_ROOT_WORD_LEN 128
#define MAX_DEFINITION_LEN 512

// Root word entry
typedef struct RootWord {
    char word[MAX_ROOT_WORD_LEN];
    char definition[MAX_DEFINITION_LEN];
    char language[32];          // "Hiligaynon", "Cebuano", "Tagalog"
    char pos[32];               // Part of speech
    char link[256];             // Reference link
    struct RootWord* next;      // For hash table chaining
} RootWord;

// Root dictionary (hash table)
typedef struct RootDictionary {
    RootWord** buckets;
    unsigned int size;
    unsigned int count;
    char language[32];          // Default language of this dictionary
} RootDictionary;

// Global root dictionaries
extern RootDictionary* g_bisaya_roots;
extern RootDictionary* g_tagalog_roots;


// Load root dictionaries from JSON files
bool load_root_dictionaries(const char* data_dir);

// Load individual root dictionaries
bool load_bisaya_roots_json(const char* filepath);
bool load_tagalog_roots_json(const char* filepath);


// Lookup functions for roots
RootWord* lookup_bisaya_root(const char* word);
RootWord* lookup_tagalog_root(const char* word);

RootWord* lookup_any_root(const char* word, const char* language);

// Get root count
unsigned int get_bisaya_root_count(void);
unsigned int get_tagalog_root_count(void);


// Cleanup root dictionaries
void cleanup_root_dictionaries(void);

// Debug/Info for roots
void print_root_stats(void);

#endif
