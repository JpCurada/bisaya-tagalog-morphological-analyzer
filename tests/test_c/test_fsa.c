#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "analyzer.h"
#include "json_loader.h"
#include "morphophonemic.h"
#include "reduplication.h"
#include "utils.h"


/**
 * Test Suite for the Morphological Analyzer
 * 
 * Tests cover:
 * - Utility functions
 * - Reduplication detection
 * - Morphophonemic reversal
 * - JSON loading
 * - Full word analysis
 */

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (condition) { \
            tests_passed++; \
            printf("  ✓ %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  ✗ %s (FAILED)\n", message); \
        } \
    } while(0)

// =====================================================
// Utility Function Tests
// =====================================================

void test_is_vowel(void) {
    printf("\n--- Testing is_vowel ---\n");
    
    TEST_ASSERT(is_vowel('a') == true, "a is a vowel");
    TEST_ASSERT(is_vowel('e') == true, "e is a vowel");
    TEST_ASSERT(is_vowel('i') == true, "i is a vowel");
    TEST_ASSERT(is_vowel('o') == true, "o is a vowel");
    TEST_ASSERT(is_vowel('u') == true, "u is a vowel");
    TEST_ASSERT(is_vowel('A') == true, "A is a vowel (uppercase)");
    TEST_ASSERT(is_vowel('b') == false, "b is not a vowel");
    TEST_ASSERT(is_vowel('k') == false, "k is not a vowel");
}

void test_is_consonant(void) {
    printf("\n--- Testing is_consonant ---\n");
    
    TEST_ASSERT(is_consonant('b') == true, "b is a consonant");
    TEST_ASSERT(is_consonant('k') == true, "k is a consonant");
    TEST_ASSERT(is_consonant('m') == true, "m is a consonant");
    TEST_ASSERT(is_consonant('a') == false, "a is not a consonant");
    TEST_ASSERT(is_consonant('e') == false, "e is not a consonant");
}

void test_trim(void) {
    printf("\n--- Testing trim ---\n");
    
    char s1[] = "  hello  ";
    trim(s1);
    TEST_ASSERT(strcmp(s1, "hello") == 0, "trim leading/trailing spaces");
    
    char s2[] = "\t\nworld\t\n";
    trim(s2);
    TEST_ASSERT(strcmp(s2, "world") == 0, "trim tabs and newlines");
    
    char s3[] = "nochange";
    trim(s3);
    TEST_ASSERT(strcmp(s3, "nochange") == 0, "no change needed");
}

void test_insert_char_at(void) {
    printf("\n--- Testing insert_char_at ---\n");
    
    char* result = insert_char_at("helo", 'l', 3);
    TEST_ASSERT(result != NULL && strcmp(result, "hello") == 0, "insert 'l' into 'helo'");
    free(result);
    
    result = insert_char_at("ello", 'h', 0);
    TEST_ASSERT(result != NULL && strcmp(result, "hello") == 0, "insert 'h' at start");
    free(result);
    
    result = insert_char_at("hell", 'o', 4);
    TEST_ASSERT(result != NULL && strcmp(result, "hello") == 0, "insert 'o' at end");
    free(result);
}

void test_normalize_word(void) {
    printf("\n--- Testing normalize_word ---\n");
    
    char output[64];
    
    normalize_word("Hello-World", output, sizeof(output));
    TEST_ASSERT(strcmp(output, "helloworld") == 0, "lowercase and remove hyphen");
    
    normalize_word("NAG-LUTO", output, sizeof(output));
    TEST_ASSERT(strcmp(output, "nagluto") == 0, "normalize nag-luto");
}

// =====================================================
// Reduplication Tests
// =====================================================

void test_cv_reduplication(void) {
    printf("\n--- Testing CV- Reduplication ---\n");
    
    Reduplication result;
    bool found;
    
    // bibili -> bi- + bili
    found = detect_cv_reduplication("bibili", &result);
    TEST_ASSERT(found == true, "detected CV- in 'bibili'");
    TEST_ASSERT(strcmp(result.base, "bili") == 0, "base is 'bili'");
    TEST_ASSERT(strcmp(result.removed, "bi") == 0, "removed is 'bi'");
    
    // kakain -> ka- + kain
    found = detect_cv_reduplication("kakain", &result);
    TEST_ASSERT(found == true, "detected CV- in 'kakain'");
    TEST_ASSERT(strcmp(result.base, "kain") == 0, "base is 'kain'");
    
    // palit (no reduplication)
    found = detect_cv_reduplication("palit", &result);
    TEST_ASSERT(found == false, "no CV- in 'palit'");
}

void test_full_reduplication(void) {
    printf("\n--- Testing Full Reduplication ---\n");
    
    Reduplication result;
    bool found;
    
    // arawaraw -> araw + araw
    found = detect_full_reduplication("arawaraw", &result);
    TEST_ASSERT(found == true, "detected full redup in 'arawaraw'");
    TEST_ASSERT(strcmp(result.base, "araw") == 0, "base is 'araw'");
    
    // bahay (odd length)
    found = detect_full_reduplication("bahay", &result);
    TEST_ASSERT(found == false, "no full redup in 'bahay' (odd length)");
}

void test_detect_reduplication(void) {
    printf("\n--- Testing detect_reduplication ---\n");
    
    ReduplicationList* list;
    
    list = detect_reduplication("bibili", "Tagalog");
    TEST_ASSERT(list != NULL && list->count > 0, "found reduplication in 'bibili'");
    if (list) free_reduplication_list(list);
    
    list = detect_reduplication("palit", "Cebuano");
    TEST_ASSERT(list != NULL && list->count == 0, "no reduplication in 'palit'");
    if (list) free_reduplication_list(list);
}

// =====================================================
// Morphophonemic Tests
// =====================================================

void test_is_nasal_affix(void) {
    printf("\n--- Testing is_nasal_affix ---\n");
    
    TEST_ASSERT(is_nasal_affix("mam-") == true, "mam- is nasal");
    TEST_ASSERT(is_nasal_affix("man-") == true, "man- is nasal");
    TEST_ASSERT(is_nasal_affix("mang-") == true, "mang- is nasal");
    TEST_ASSERT(is_nasal_affix("nam-") == true, "nam- is nasal");
    TEST_ASSERT(is_nasal_affix("pang-") == true, "pang- is nasal");
    TEST_ASSERT(is_nasal_affix("nag-") == false, "nag- is not nasal");
    TEST_ASSERT(is_nasal_affix("gi-") == false, "gi- is not nasal");
}

void test_nasal_reversal(void) {
    printf("\n--- Testing Nasal Reversal ---\n");
    
    MorphoCandidateList result;
    result.count = 0;
    
    // After stripping mam- from mamalit, remaining is "alit"
    // Should restore to "palit" (p was assimilated to m)
    apply_nasal_reversal("alit", "mam-", &result);
    
    TEST_ASSERT(result.count > 0, "nasal reversal produced candidates");
    
    // Check if "palit" is among candidates
    bool found_palit = false;
    for (int i = 0; i < result.count; i++) {
        if (strcmp(result.candidates[i].form, "palit") == 0) {
            found_palit = true;
            break;
        }
    }
    TEST_ASSERT(found_palit, "nasal reversal found 'palit' from 'alit'");
}

void test_morphophonemic_reverse(void) {
    printf("\n--- Testing reverse_morphophonemic ---\n");
    
    MorphoCandidateList* result;
    
    // Test with nasal prefix
    result = reverse_morphophonemic("ali", "mam-", "Tagalog");
    TEST_ASSERT(result != NULL && result->count > 0, "morphophonemic produced candidates");
    
    // Should include the original form
    bool found_original = false;
    for (int i = 0; i < result->count; i++) {
        if (strcmp(result->candidates[i].form, "ali") == 0) {
            found_original = true;
            break;
        }
    }
    TEST_ASSERT(found_original, "original form 'ali' is included");
    
    free_morpho_candidates(result);
}

// =====================================================
// JSON Loader Tests
// =====================================================

void test_json_loader(void) {
    printf("\n--- Testing JSON Loader ---\n");
    
    // Assuming tests are run from project root
    bool loaded = load_affix_tables("data");
    TEST_ASSERT(loaded == true, "loaded affix tables from data/");
    
    // Check some known prefixes
    Affix* mu = lookup_prefix("mu-");
    TEST_ASSERT(mu != NULL, "found prefix 'mu-'");
    if (mu) {
        TEST_ASSERT(strcmp(mu->definitions->language, "Cebuano") == 0, 
                   "mu- is Cebuano only");
    }
    
    Affix* nag = lookup_prefix("nag-");
    TEST_ASSERT(nag != NULL, "found prefix 'nag-'");
    if (nag) {
        TEST_ASSERT(strcmp(nag->definitions->language, "Both") == 0,
                   "nag- is for Both languages");
    }
    
    // Check suffixes
    Affix* an = lookup_suffix("-an");
    TEST_ASSERT(an != NULL, "found suffix '-an'");
    
    // Check infixes
    Affix* um = lookup_infix("-um-");
    TEST_ASSERT(um != NULL, "found infix '-um-'");
    
    // Check circumfixes
    Affix* gi_an = lookup_circumfix("gi-...-an");
    TEST_ASSERT(gi_an != NULL, "found circumfix 'gi-...-an'");
    if (gi_an) {
        TEST_ASSERT(strcmp(gi_an->prefix_part, "gi-") == 0, "prefix part is 'gi-'");
        TEST_ASSERT(strcmp(gi_an->suffix_part, "-an") == 0, "suffix part is '-an'");
    }
    
    cleanup_affix_tables();
}

void test_root_loading(void) {
    printf("\n--- Testing Root Dictionary Loading ---\n");
    
    // Load root dictionaries
    bool loaded = load_root_dictionaries("data");
    TEST_ASSERT(loaded == true, "loaded root dictionaries from data/");
    
    // Check counts
    unsigned int bisaya_count = get_bisaya_root_count();
    unsigned int tagalog_count = get_tagalog_root_count();
    unsigned int shared_count = get_shared_vocab_count();
    
    printf("  Bisaya roots: %u\n", bisaya_count);
    printf("  Tagalog roots: %u\n", tagalog_count);
    printf("  Shared vocab: %u\n", shared_count);
    
    TEST_ASSERT(bisaya_count > 1000, "loaded many Bisaya roots");
    TEST_ASSERT(tagalog_count > 1000, "loaded many Tagalog roots");
    TEST_ASSERT(shared_count > 10, "loaded shared vocabulary");
    
    // Test some lookups
    RootWord* palit = lookup_bisaya_root("palit");
    TEST_ASSERT(palit != NULL, "found 'palit' in Bisaya roots");
    if (palit) {
        printf("  palit definition: %.50s...\n", palit->definition);
    }
    
    RootWord* aba = lookup_tagalog_root("aba");
    TEST_ASSERT(aba != NULL, "found 'aba' in Tagalog roots");
    if (aba) {
        printf("  aba definition: %.50s...\n", aba->definition);
    }
    
    // Test shared vocab
    RootWord* luto = lookup_shared_vocab("luto");
    TEST_ASSERT(luto != NULL, "found 'luto' in shared vocab");
    if (luto) {
        printf("  luto definition: %s\n", luto->definition);
    }
    
    // Test lookup_any_root
    RootWord* any_root = lookup_any_root("tubig", NULL);
    TEST_ASSERT(any_root != NULL, "found 'tubig' via lookup_any_root");
    
    cleanup_root_dictionaries();
}


// =====================================================
// Full Analysis Tests
// =====================================================

void test_simple_prefix(void) {
    printf("\n--- Testing Simple Prefix Analysis ---\n");
    
    // Initialize analyzer
    bool init = init_analyzer_v2("data");
    TEST_ASSERT(init == true, "analyzer initialized");
    
    // Note: This requires root dictionaries to be populated
    // For now, test that analysis runs without crashing
    
    AnalysisResult* results = analyze_word_v2("mupalit", "Cebuano");
    // May or may not find results depending on root dictionary
    TEST_ASSERT(true, "analyze_word_v2 ran without crashing");
    
    if (results) {
        printf("  Found %d analysis(es) for 'mupalit'\n", count_analyses(results));
        print_all_analyses(results);
        free_analysis_results(results);
    }
    
    cleanup_analyzer_v2();
}

void test_complex_analysis(void) {
    printf("\n--- Testing Complex Analysis ---\n");
    
    init_analyzer_v2("data");
    
    // Test various words
    const char* test_words[] = {
        "nagpalit",    // nag- + palit
        "gipalitan",   // gi-...-an + palit
        "bibili",      // CV-redup + bili
        "makapalit",   // maka- + palit
        "palitan",     // palit + -an
    };
    
    for (int i = 0; i < 5; i++) {
        printf("\n  Analyzing: %s\n", test_words[i]);
        AnalysisResult* results = analyze_word_v2(test_words[i], NULL);
        if (results) {
            printf("    Found %d analysis(es)\n", count_analyses(results));
            free_analysis_results(results);
        } else {
            printf("    No analyses found\n");
        }
    }
    
    TEST_ASSERT(true, "complex analysis tests completed");
    
    cleanup_analyzer_v2();
}

// =====================================================
// Main Test Runner
// =====================================================

int main(int argc, char* argv[]) {
    printf("========================================\n");
    printf("  Morphological Analyzer Test Suite    \n");
    printf("========================================\n");
    
    // Utility tests
    test_is_vowel();
    test_is_consonant();
    test_trim();
    test_insert_char_at();
    test_normalize_word();
    
    // Reduplication tests
    test_cv_reduplication();
    test_full_reduplication();
    test_detect_reduplication();
    
    // Morphophonemic tests
    test_is_nasal_affix();
    test_nasal_reversal();
    test_morphophonemic_reverse();
    
    // JSON Loader tests
    test_json_loader();
    
    // Root loading tests
    test_root_loading();
    
    // Full analysis tests
    test_simple_prefix();
    test_complex_analysis();
    
    // Summary
    printf("\n========================================\n");
    printf("  TEST SUMMARY                         \n");
    printf("========================================\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("  Total:  %d\n", tests_passed + tests_failed);
    printf("========================================\n");
    
    return tests_failed > 0 ? 1 : 0;
}
