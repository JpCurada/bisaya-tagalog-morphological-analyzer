#include "morphology.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Helper to extract field from pipe-delimited string "Language|POS|Extra"
static void get_field(const char* val, int index, char* buffer, size_t size) {
    if (!val || !buffer || size == 0) return;
    buffer[0] = '\0';
    
    char temp[256];
    strncpy(temp, val, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char* token = strtok(temp, "|");
    int i = 0;
    while (token != NULL) {
        if (i == index) {
            strncpy(buffer, token, size - 1);
            buffer[size - 1] = '\0';
            return;
        }
        token = strtok(NULL, "|");
        i++;
    }
}

Language parse_language(const char* lang_str) {
    if (!lang_str) return LANG_UNKNOWN;
    if (strcmp(lang_str, "Bisaya") == 0) return LANG_BISAYA;
    if (strcmp(lang_str, "Tagalog") == 0) return LANG_TAGALOG;
    if (strcmp(lang_str, "Both") == 0) return LANG_BOTH;
    return LANG_UNKNOWN;
}

const char* language_to_string(Language lang) {
    switch (lang) {
        case LANG_BISAYA: return "Bisaya";
        case LANG_TAGALOG: return "Tagalog";
        case LANG_BOTH: return "Shared";
        default: return "Unknown";
    }
}

Language get_root_language(const char* root_val) {
    char lang_buf[32];
    get_field(root_val, 0, lang_buf, sizeof(lang_buf));
    return parse_language(lang_buf);
}

bool validate_morphology(const char* prefix_val, const char* root_val, const char* suffix_val) {
    if (!root_val) return false;
    
    char root_lang_str[32], root_pos[32];
    get_field(root_val, 0, root_lang_str, sizeof(root_lang_str));
    get_field(root_val, 1, root_pos, sizeof(root_pos));
    
    Language root_lang = parse_language(root_lang_str);
    
    // 1. Check Prefix Compatibility
    if (prefix_val) {
        // If new format (no pipes), assume valid for now to allow rich output
        if (strchr(prefix_val, '|') == NULL) {
             // Pass
        } else {
            char pre_lang_str[32], pre_pos_list[128];
            get_field(prefix_val, 0, pre_lang_str, sizeof(pre_lang_str));
            get_field(prefix_val, 1, pre_pos_list, sizeof(pre_pos_list));
            
            Language pre_lang = parse_language(pre_lang_str);
            
            // Language check
            if (pre_lang != LANG_BOTH && root_lang != LANG_BOTH && pre_lang != root_lang) {
                return false; // Language mismatch
            }
            
            // POS check (simple substring check for now)
            if (strstr(pre_pos_list, root_pos) == NULL) {
                 return false;
            }
        }
    }
    
    // 2. Check Suffix Compatibility
    if (suffix_val) {
        if (strchr(suffix_val, '|') == NULL) {
             // Pass
        } else {
            char suf_lang_str[32], suf_pos_list[128];
            get_field(suffix_val, 0, suf_lang_str, sizeof(suf_lang_str));
            get_field(suffix_val, 1, suf_pos_list, sizeof(suf_pos_list));
            
            Language suf_lang = parse_language(suf_lang_str);
            
            if (suf_lang != LANG_BOTH && root_lang != LANG_BOTH && suf_lang != root_lang) {
                return false;
            }
            
            if (strstr(suf_pos_list, root_pos) == NULL) {
                return false;
            }
        }
    }
    
    return true;
}
