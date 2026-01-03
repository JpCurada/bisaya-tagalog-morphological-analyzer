#ifndef REDUPLICATION_H
#define REDUPLICATION_H

#include <stdbool.h>

/**
 * Reduplication Detection Module for Philippine Languages
 * 
 * Handles three types of reduplication:
 * - CV- reduplication (e.g., bi-bili from bili)
 * - CVC- reduplication (e.g., sul-sulat from sulat)  
 * - Full reduplication (e.g., araw-araw from araw)
 */

#define MAX_REDUP_PATTERNS 8

// Reduplication pattern types
typedef enum {
    REDUP_NONE = 0,
    REDUP_CV,           // First consonant + vowel: bi-bili
    REDUP_CVC,          // First syllable: sul-sulat
    REDUP_FULL,         // Complete word: araw-araw
    REDUP_CV_GLOTTAL    // CV with glottal: la-la?
} RedupType;

// Single reduplication pattern result
typedef struct Reduplication {
    RedupType type;
    char pattern_name[16];      // "CV-", "CVC-", "FULL"
    char removed[64];           // The reduplicated portion
    char base[256];             // Remaining base after removing reduplication
    float confidence;           // How confident we are this is reduplication
} Reduplication;

// List of detected reduplication patterns
typedef struct ReduplicationList {
    Reduplication patterns[MAX_REDUP_PATTERNS];
    int count;
} ReduplicationList;

/**
 * Detect reduplication patterns in a word.
 * Returns all possible reduplication interpretations.
 * 
 * @param word      The word to analyze
 * @param language  "Cebuano", "Tagalog", or "Both"
 * @return          List of possible reduplication patterns
 */
ReduplicationList* detect_reduplication(const char* word, const char* language);

/**
 * Check if word has CV- reduplication pattern.
 * CV- reduplication copies the first consonant and vowel.
 * 
 * Examples:
 * - bibili → CV = bi, base = bili
 * - kakain → CV = ka, base = kain
 */
bool detect_cv_reduplication(const char* word, Reduplication* result);

/**
 * Check if word has CVC- reduplication pattern.
 * CVC- reduplication copies the first syllable.
 * 
 * Examples:
 * - sulsulat → CVC = sul, base = sulat
 */
bool detect_cvc_reduplication(const char* word, Reduplication* result);

/**
 * Check if word has full reduplication pattern.
 * The word is the same pattern repeated twice.
 * 
 * Examples:
 * - arawaraw → araw + araw
 * - bahay-bahay → bahay + bahay
 */
bool detect_full_reduplication(const char* word, Reduplication* result);

/**
 * Get human-readable description of reduplication type.
 */
const char* redup_type_to_string(RedupType type);

/**
 * Free reduplication list.
 */
void free_reduplication_list(ReduplicationList* list);

#endif
