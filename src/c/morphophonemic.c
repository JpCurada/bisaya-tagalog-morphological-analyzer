#include "morphophonemic.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Helper to add a candidate to the list
static void add_candidate(MorphoCandidateList* list, const char* form, 
                          const char* changes, float confidence) {
    if (list->count >= MAX_MORPHO_CANDIDATES) return;
    
    safe_strcpy(list->candidates[list->count].form, form, sizeof(list->candidates[0].form));
    safe_strcpy(list->candidates[list->count].changes, changes, sizeof(list->candidates[0].changes));
    list->candidates[list->count].confidence = confidence;
    list->count++;
}

// Check if string starts with given prefix
static bool str_starts_with(const char* str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

bool is_nasal_affix(const char* affix_key) {
    if (!affix_key) return false;
    
    // Check for maN- series (future)
    if (str_starts_with(affix_key, "mam") ||
        str_starts_with(affix_key, "man") ||
        str_starts_with(affix_key, "mang") ||
        str_starts_with(affix_key, "mangl") ||
        str_starts_with(affix_key, "manl") ||
        str_starts_with(affix_key, "many")) {
        return true;
    }
    
    // Check for naN- series (past)
    if (str_starts_with(affix_key, "nam") ||
        str_starts_with(affix_key, "nan") ||
        str_starts_with(affix_key, "nang") ||
        str_starts_with(affix_key, "nangl") ||
        str_starts_with(affix_key, "nanl") ||
        str_starts_with(affix_key, "nany")) {
        return true;
    }
    
    // Check for paN- series (instrumental)
    if (str_starts_with(affix_key, "pam") ||
        str_starts_with(affix_key, "pan") ||
        str_starts_with(affix_key, "pang") ||
        str_starts_with(affix_key, "pany")) {
        return true;
    }
    
    // Check for ipaN- series (instrumental past)
    if (str_starts_with(affix_key, "ipam") ||
        str_starts_with(affix_key, "ipan") ||
        str_starts_with(affix_key, "ipang") ||
        str_starts_with(affix_key, "ipany")) {
        return true;
    }
    
    return false;
}

int apply_nasal_reversal(const char* base, const char* nasal_prefix, 
                         MorphoCandidateList* result) {
    if (!base || !nasal_prefix || !result) return 0;
    
    int added = 0;
    char new_form[256];
    char change_desc[256];
    
    // Determine which consonants were assimilated based on prefix ending
    
    // mam-/nam-/pam-/ipam- : p, b → m (bilabial assimilation)
    if (str_starts_with(nasal_prefix, "mam") || 
        str_starts_with(nasal_prefix, "nam") ||
        str_starts_with(nasal_prefix, "pam") ||
        str_starts_with(nasal_prefix, "ipam") ||
        str_starts_with(nasal_prefix, "ipinam")) {
        
        // Try restoring p
        snprintf(new_form, sizeof(new_form), "p%s", base);
        snprintf(change_desc, sizeof(change_desc), "nasal_reversal:p_restored_from_m");
        add_candidate(result, new_form, change_desc, 0.9f);
        added++;
        
        // Try restoring b
        snprintf(new_form, sizeof(new_form), "b%s", base);
        snprintf(change_desc, sizeof(change_desc), "nasal_reversal:b_restored_from_m");
        add_candidate(result, new_form, change_desc, 0.85f);
        added++;
    }
    
    // man-/nan-/pan-/ipan- : t, d, s, r, l → n (dental/alveolar assimilation)
    if (str_starts_with(nasal_prefix, "man") || 
        str_starts_with(nasal_prefix, "nan") ||
        str_starts_with(nasal_prefix, "pan") ||
        str_starts_with(nasal_prefix, "ipan") ||
        str_starts_with(nasal_prefix, "ipinan")) {
        
        const char* consonants[] = {"t", "d", "s", "r", "l"};
        float confidences[] = {0.9f, 0.85f, 0.85f, 0.7f, 0.7f};
        
        for (int i = 0; i < 5; i++) {
            snprintf(new_form, sizeof(new_form), "%s%s", consonants[i], base);
            snprintf(change_desc, sizeof(change_desc), "nasal_reversal:%s_restored_from_n", consonants[i]);
            add_candidate(result, new_form, change_desc, confidences[i]);
            added++;
        }
    }
    
    // mang-/nang-/pang- : k, h, glottal, vowel-initial → ng (velar assimilation)
    if (str_starts_with(nasal_prefix, "mang") || 
        str_starts_with(nasal_prefix, "nang") ||
        str_starts_with(nasal_prefix, "pang") ||
        str_starts_with(nasal_prefix, "ipang") ||
        str_starts_with(nasal_prefix, "ipinang")) {
        
        // Try restoring k
        snprintf(new_form, sizeof(new_form), "k%s", base);
        snprintf(change_desc, sizeof(change_desc), "nasal_reversal:k_restored_from_ng");
        add_candidate(result, new_form, change_desc, 0.9f);
        added++;
        
        // Try restoring h
        snprintf(new_form, sizeof(new_form), "h%s", base);
        snprintf(change_desc, sizeof(change_desc), "nasal_reversal:h_restored_from_ng");
        add_candidate(result, new_form, change_desc, 0.7f);
        added++;
        
        // Could be vowel-initial (no consonant deleted)
        add_candidate(result, base, "nasal_reversal:vowel_initial", 0.6f);
        added++;
    }
    
    // mangl-/nangl-/manl-/nanl- : Cebuano /l/ preservation
    if (str_starts_with(nasal_prefix, "mangl") || 
        str_starts_with(nasal_prefix, "nangl") ||
        str_starts_with(nasal_prefix, "manl") ||
        str_starts_with(nasal_prefix, "nanl")) {
        
        // The base already has l, just confirm
        snprintf(new_form, sizeof(new_form), "l%s", base);
        snprintf(change_desc, sizeof(change_desc), "nasal_reversal:l_preserved");
        add_candidate(result, new_form, change_desc, 0.95f);
        added++;
    }
    
    return added;
}

int apply_l_insertion(const char* base, MorphoCandidateList* result) {
    if (!base || !result) return 0;
    
    int added = 0;
    size_t len = strlen(base);
    
    // Look for patterns where /l/ might have been dropped
    // Common pattern: doubled vowels (aa, ii, oo, etc.)
    for (size_t i = 0; i < len - 1; i++) {
        if (is_vowel(base[i]) && base[i] == base[i+1]) {
            // Possible /l/-dropping site
            char* new_form = insert_char_at(base, 'l', (int)(i + 1));
            if (new_form) {
                char change_desc[128];
                snprintf(change_desc, sizeof(change_desc), "l_insertion:position_%zu", i + 1);
                add_candidate(result, new_form, change_desc, 0.7f);
                free(new_form);
                added++;
            }
        }
    }
    
    // Look for CV sequences where l might have been dropped between C and V
    for (size_t i = 0; i < len - 1; i++) {
        if (is_consonant(base[i]) && is_vowel(base[i+1])) {
            // Could have had an /l/ between consonant and vowel
            char* new_form = insert_char_at(base, 'l', (int)(i + 1));
            if (new_form) {
                char change_desc[128];
                snprintf(change_desc, sizeof(change_desc), "l_insertion:after_consonant_%zu", i);
                add_candidate(result, new_form, change_desc, 0.5f);
                free(new_form);
                added++;
            }
        }
    }
    
    return added;
}

int apply_vowel_restoration(const char* base, MorphoCandidateList* result) {
    if (!base || !result) return 0;
    
    int added = 0;
    size_t len = strlen(base);
    
    // Look for consonant clusters that might have had a vowel syncopated
    for (size_t i = 0; i < len - 1; i++) {
        if (is_consonant(base[i]) && is_consonant(base[i+1])) {
            // Try inserting common vowels
            const char vowels[] = {'a', 'i', 'u', 'o', 'e'};
            const float confidences[] = {0.6f, 0.5f, 0.4f, 0.3f, 0.2f};
            
            for (int v = 0; v < 5; v++) {
                char* new_form = insert_char_at(base, vowels[v], (int)(i + 1));
                if (new_form) {
                    char change_desc[128];
                    snprintf(change_desc, sizeof(change_desc), "vowel_restoration:%c_at_%zu", 
                             vowels[v], i + 1);
                    add_candidate(result, new_form, change_desc, confidences[v]);
                    free(new_form);
                    added++;
                }
            }
        }
    }
    
    return added;
}

int apply_metathesis_reversal(const char* base, MorphoCandidateList* result) {
    if (!base || !result) return 0;
    
    int added = 0;
    size_t len = strlen(base);
    
    // Look for CV patterns that might be reversed VC patterns
    for (size_t i = 0; i < len - 1; i++) {
        // Try swapping adjacent characters
        if ((is_consonant(base[i]) && is_vowel(base[i+1])) ||
            (is_vowel(base[i]) && is_consonant(base[i+1]))) {
            
            char new_form[256];
            safe_strcpy(new_form, base, sizeof(new_form));
            
            // Swap characters
            char temp = new_form[i];
            new_form[i] = new_form[i+1];
            new_form[i+1] = temp;
            
            char change_desc[128];
            snprintf(change_desc, sizeof(change_desc), "metathesis_reversal:swap_%zu_%zu", i, i+1);
            add_candidate(result, new_form, change_desc, 0.4f);
            added++;
        }
    }
    
    return added;
}

int apply_liquid_alternation(const char* base, MorphoCandidateList* result) {
    if (!base || !result) return 0;
    
    int added = 0;
    size_t len = strlen(base);
    
    // d/r/l alternations are common in Cebuano
    const char liquids[] = {'d', 'r', 'l'};
    
    for (size_t i = 0; i < len; i++) {
        char c = base[i];
        
        // Check if current character is a liquid
        if (c == 'd' || c == 'r' || c == 'l') {
            // Try replacing with each alternative
            for (int j = 0; j < 3; j++) {
                if (liquids[j] != c) {
                    char new_form[256];
                    safe_strcpy(new_form, base, sizeof(new_form));
                    new_form[i] = liquids[j];
                    
                    char change_desc[128];
                    snprintf(change_desc, sizeof(change_desc), 
                             "liquid_alternation:%c_to_%c_at_%zu", c, liquids[j], i);
                    add_candidate(result, new_form, change_desc, 0.5f);
                    added++;
                }
            }
        }
    }
    
    return added;
}

int apply_h_removal(const char* base, MorphoCandidateList* result) {
    if (!base || !result) return 0;
    
    int added = 0;
    size_t len = strlen(base);
    
    // Look for /h/ between vowels (intervocalic h)
    for (size_t i = 1; i < len - 1; i++) {
        if (base[i] == 'h' && is_vowel(base[i-1]) && is_vowel(base[i+1])) {
            // Remove the /h/
            char* new_form = remove_char_at(base, (int)i);
            if (new_form) {
                add_candidate(result, new_form, "h_removal:intervocalic", 0.7f);
                free(new_form);
                added++;
            }
        }
    }
    
    // Also check for word-final h after vowel
    if (len > 1 && base[len-1] == 'h' && is_vowel(base[len-2])) {
        char* new_form = remove_char_at(base, (int)(len - 1));
        if (new_form) {
            add_candidate(result, new_form, "h_removal:word_final", 0.6f);
            free(new_form);
            added++;
        }
    }
    
    return added;
}

MorphoCandidateList* reverse_morphophonemic(const char* base, const char* affix_key, 
                                             const char* language) {
    if (!base) return NULL;
    
    MorphoCandidateList* result = (MorphoCandidateList*)calloc(1, sizeof(MorphoCandidateList));
    if (!result) return NULL;
    
    result->count = 0;
    
    // Always include the base form as-is (no changes)
    add_candidate(result, base, "", 1.0f);
    
    // Apply nasal assimilation reversal if applicable
    if (affix_key && is_nasal_affix(affix_key)) {
        apply_nasal_reversal(base, affix_key, result);
    }
    
    // Apply language-specific rules
    bool is_cebuano = !language || strcmp(language, "Cebuano") == 0 || strcmp(language, "Both") == 0;
    bool is_tagalog = !language || strcmp(language, "Tagalog") == 0 || strcmp(language, "Both") == 0;
    
    if (is_cebuano) {
        // Cebuano-specific rules
        apply_l_insertion(base, result);
        apply_liquid_alternation(base, result);
        // Note: Metathesis and vowel restoration generate many candidates
        // Only apply if we haven't found valid roots yet (can be controlled by caller)
    }
    
    if (is_tagalog) {
        // Tagalog-specific rules
        apply_h_removal(base, result);
    }
    
    return result;
}

void free_morpho_candidates(MorphoCandidateList* list) {
    if (list) {
        free(list);
    }
}
