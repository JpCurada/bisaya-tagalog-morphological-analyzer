#ifndef MORPHOPHONEMIC_H
#define MORPHOPHONEMIC_H

#include <stdbool.h>

/**
 * Morphophonemic Engine for Philippine Languages
 * 
 * Handles phonological rules that occur at morpheme boundaries:
 * - Nasal assimilation (maN- + palit → mamalit)
 * - /l/-dropping (Cebuano)
 * - Vowel deletion
 * - Metathesis
 * - Liquid alternations (d/r/l)
 * - /h/-insertion
 */

#define MAX_MORPHO_CANDIDATES 32

// Morphophonemic candidate (possible original form)
typedef struct MorphoCandidate {
    char form[256];             // Reconstructed form
    char changes[512];          // Description of changes applied
    float confidence;           // Confidence score (0.0 - 1.0)
} MorphoCandidate;

// Result array with count
typedef struct MorphoCandidateList {
    MorphoCandidate candidates[MAX_MORPHO_CANDIDATES];
    int count;
} MorphoCandidateList;

/**
 * Reverse morphophonemic changes to find possible original root forms.
 * 
 * @param base          The remaining string after affix stripping
 * @param affix_key     The affix that was stripped (e.g., "mam-")
 * @param language      "Cebuano", "Tagalog", or "Both"
 * @return              List of possible original forms
 */
MorphoCandidateList* reverse_morphophonemic(
    const char* base,
    const char* affix_key,
    const char* language
);

/**
 * Apply nasal assimilation reversal.
 * Restores consonants that were assimilated by nasal prefixes.
 * 
 * Examples:
 * - mam- + alit → mamalit (from palit, p→m)
 * - man- + ulat → manulat (from sulat, s→n)
 * - mang- + isda → mangisda (k-deletion or vowel-initial)
 */
int apply_nasal_reversal(
    const char* base,
    const char* nasal_prefix,
    MorphoCandidateList* result
);

/**
 * Apply /l/-dropping reversal (Cebuano).
 * Restores /l/ that was dropped in certain environments.
 * 
 * Examples:
 * - baay → balay
 * - kinahanglan may have /l/ restored in certain positions
 */
int apply_l_insertion(
    const char* base,
    MorphoCandidateList* result
);

/**
 * Apply vowel restoration (Cebuano).
 * Restores vowels that were syncopated before suffixes.
 */
int apply_vowel_restoration(
    const char* base,
    MorphoCandidateList* result
);

/**
 * Apply metathesis reversal (Cebuano).
 * Reverses consonant/vowel swapping.
 */
int apply_metathesis_reversal(
    const char* base,
    MorphoCandidateList* result
);

/**
 * Apply liquid alternation reversal.
 * Handles d/r/l alternations common in Cebuano.
 */
int apply_liquid_alternation(
    const char* base,
    MorphoCandidateList* result
);

/**
 * Apply /h/-removal (Tagalog).
 * Removes /h/ that was inserted between vowels.
 */
int apply_h_removal(
    const char* base,
    MorphoCandidateList* result
);

/**
 * Check if an affix triggers nasal assimilation.
 */
bool is_nasal_affix(const char* affix_key);

/**
 * Free morphophonemic candidate list.
 */
void free_morpho_candidates(MorphoCandidateList* list);

#endif
