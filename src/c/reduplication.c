#include "reduplication.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char* redup_type_to_string(RedupType type) {
    switch (type) {
        case REDUP_CV:      return "CV-";
        case REDUP_CVC:     return "CVC-";
        case REDUP_FULL:    return "FULL";
        case REDUP_CV_GLOTTAL: return "CV-?";
        default:            return "NONE";
    }
}

bool detect_cv_reduplication(const char* word, Reduplication* result) {
    if (!word || !result) return false;
    
    size_t len = strlen(word);
    
    // Minimum length for CV- reduplication: 4 (CV + CV)
    // e.g., "bibi" would be minimum
    if (len < 4) return false;
    
    // Extract first two characters (potential CV pattern)
    // Check that first is consonant and second is vowel
    if (!is_consonant(word[0]) || !is_vowel(word[1])) {
        return false;
    }
    
    char cv_pattern[3] = {word[0], word[1], '\0'};
    
    // Check if the next two characters match the CV pattern
    if (word[2] == word[0] && word[3] == word[1]) {
        // Found CV- reduplication
        result->type = REDUP_CV;
        safe_strcpy(result->pattern_name, "CV-", sizeof(result->pattern_name));
        safe_strcpy(result->removed, cv_pattern, sizeof(result->removed));
        safe_strcpy(result->base, word + 2, sizeof(result->base));
        
        // Higher confidence if resulting base is reasonable length
        size_t base_len = len - 2;
        if (base_len >= 3 && base_len <= 10) {
            result->confidence = 0.9f;
        } else {
            result->confidence = 0.7f;
        }
        
        return true;
    }
    
    return false;
}

bool detect_cvc_reduplication(const char* word, Reduplication* result) {
    if (!word || !result) return false;
    
    size_t len = strlen(word);
    
    // Minimum length for CVC- reduplication: 6 (CVC + CVC)
    if (len < 6) return false;
    
    // Check for CVC pattern at start
    if (!is_consonant(word[0]) || !is_vowel(word[1]) || !is_consonant(word[2])) {
        return false;
    }
    
    char cvc_pattern[4] = {word[0], word[1], word[2], '\0'};
    
    // Check if next three characters match the CVC pattern
    if (word[3] == word[0] && word[4] == word[1] && word[5] == word[2]) {
        // Found CVC- reduplication
        result->type = REDUP_CVC;
        safe_strcpy(result->pattern_name, "CVC-", sizeof(result->pattern_name));
        safe_strcpy(result->removed, cvc_pattern, sizeof(result->removed));
        safe_strcpy(result->base, word + 3, sizeof(result->base));
        
        // Higher confidence if resulting base is reasonable length
        size_t base_len = len - 3;
        if (base_len >= 3 && base_len <= 10) {
            result->confidence = 0.85f;
        } else {
            result->confidence = 0.65f;
        }
        
        return true;
    }
    
    return false;
}

bool detect_full_reduplication(const char* word, Reduplication* result) {
    if (!word || !result) return false;
    
    size_t len = strlen(word);
    
    // Minimum length for full reduplication: 4 (2 + 2)
    // Word length must be even
    if (len < 4 || len % 2 != 0) return false;
    
    size_t half = len / 2;
    
    // Check if first half equals second half
    if (strncmp(word, word + half, half) == 0) {
        // Found full reduplication
        result->type = REDUP_FULL;
        safe_strcpy(result->pattern_name, "FULL", sizeof(result->pattern_name));
        
        // Copy first half as the removed portion
        char removed[128];
        strncpy(removed, word, half);
        removed[half] = '\0';
        safe_strcpy(result->removed, removed, sizeof(result->removed));
        
        // Base is the second half (same as first)
        safe_strcpy(result->base, word + half, sizeof(result->base));
        
        // Higher confidence for reasonable base lengths
        if (half >= 3 && half <= 8) {
            result->confidence = 0.95f;
        } else if (half == 2) {
            result->confidence = 0.6f;  // Very short, might be coincidence
        } else {
            result->confidence = 0.75f;
        }
        
        return true;
    }
    
    return false;
}

// Detect VC- reduplication (vowel-initial words)
static bool detect_vc_reduplication(const char* word, Reduplication* result) {
    if (!word || !result) return false;
    
    size_t len = strlen(word);
    
    // Minimum length for VC- reduplication: 4
    if (len < 4) return false;
    
    // Check that first is vowel and second is consonant
    if (!is_vowel(word[0]) || !is_consonant(word[1])) {
        return false;
    }
    
    char vc_pattern[3] = {word[0], word[1], '\0'};
    
    // Check if the next two characters match
    if (word[2] == word[0] && word[3] == word[1]) {
        result->type = REDUP_CV;  // Reuse CV type for vowel-initial
        safe_strcpy(result->pattern_name, "VC-", sizeof(result->pattern_name));
        safe_strcpy(result->removed, vc_pattern, sizeof(result->removed));
        safe_strcpy(result->base, word + 2, sizeof(result->base));
        result->confidence = 0.85f;
        return true;
    }
    
    return false;
}

ReduplicationList* detect_reduplication(const char* word, const char* language) {
    (void)language; // Suppress unused parameter warning
    if (!word) return NULL;
    
    ReduplicationList* list = (ReduplicationList*)calloc(1, sizeof(ReduplicationList));
    if (!list) return NULL;
    
    list->count = 0;
    
    // Try each reduplication pattern
    // Order matters: more specific patterns first
    
    // 1. Try full reduplication (most reliable if found)
    Reduplication full_redup;
    if (detect_full_reduplication(word, &full_redup)) {
        list->patterns[list->count++] = full_redup;
    }
    
    // 2. Try CVC- reduplication (more specific than CV-)
    Reduplication cvc_redup;
    if (detect_cvc_reduplication(word, &cvc_redup)) {
        list->patterns[list->count++] = cvc_redup;
    }
    
    // 3. Try CV- reduplication
    Reduplication cv_redup;
    if (detect_cv_reduplication(word, &cv_redup)) {
        list->patterns[list->count++] = cv_redup;
    }
    
    // 4. Try VC- reduplication (for vowel-initial words)
    Reduplication vc_redup;
    if (detect_vc_reduplication(word, &vc_redup)) {
        list->patterns[list->count++] = vc_redup;
    }
    
    return list;
}

void free_reduplication_list(ReduplicationList* list) {
    if (list) {
        free(list);
    }
}
