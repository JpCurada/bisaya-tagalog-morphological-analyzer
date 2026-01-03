#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <stdbool.h>

typedef enum {
    LANG_UNKNOWN = 0,
    LANG_BISAYA,
    LANG_TAGALOG,
    LANG_BOTH
} Language;

// Convert string representation to Language enum
Language parse_language(const char* lang_str);

// Stringify language enum
const char* language_to_string(Language lang);

// Validate if prefix, root, and suffix are compatible
bool validate_morphology(const char* prefix_val, const char* root_val, const char* suffix_val);

// Get language from root value string
Language get_root_language(const char* root_val);

#endif