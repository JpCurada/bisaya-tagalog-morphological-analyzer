#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>
#include "morphology.h"
#include "json_loader.h"
#include "reduplication.h"

/**
 * Morphological Analyzer for Cebuano and Tagalog
 * 
 * Uses BFS-based Finite State Automaton to find all possible
 * morphological analyses of a word.
 */

#define MAX_WORD_LEN 256
#define MAX_AFFIXES 16
#define MAX_ANALYSES 32

// Analysis stages for FSA
typedef enum {
    STAGE_PREFIX_CHECK,
    STAGE_INFIX_CHECK,
    STAGE_SUFFIX_CHECK,
    STAGE_ROOT_CHECK,
    STAGE_DONE
} AnalysisStage;

// Matched affix info
typedef struct MatchedAffix {
    Affix* affix;               // Pointer to affix in table
    AffixDef* definition;       // Which definition was matched
    char matched_form[64];      // The actual form that was matched (allomorph)
    AffixPosition position;     // PREFIX, SUFFIX, INFIX, CIRCUMFIX
} MatchedAffix;

// Analysis state for BFS traversal
typedef struct AnalysisState {
    char remaining[MAX_WORD_LEN];       // Current remaining string to analyze
    char original[MAX_WORD_LEN];        // Original input word
    
    AnalysisStage stage;                // Current FSA stage
    
    // Parsed affixes
    MatchedAffix affixes[MAX_AFFIXES];
    int affix_count;
    
    // Morphophonemic tracking
    char morpho_changes[512];           // Record of morphophonemic changes
    
    // Reduplication tracking
    RedupType redup_type;
    char redup_removed[64];             // The reduplicated portion
    
    // Circumfix tracking
    bool has_circumfix_start;           // True if we found the prefix part of a circumfix
    char expected_suffix[32];           // The suffix we need to complete the circumfix
    struct Affix* pending_circumfix;    // Pointer to the specific circumfix definition
    int circumfix_prefix_index;         // Index of the prefix part in affixes array
    
} AnalysisState;

// Final analysis result
typedef struct AnalysisResult {
    bool valid;
    
    char original[MAX_WORD_LEN];        // Original input
    char normalized[MAX_WORD_LEN];      // Normalized form
    char root[128];                     // Identified root
    char root_gloss[256];               // Root meaning (if available)
    
    Language language;                  // Detected language
    
    // Affixes
    MatchedAffix affixes[MAX_AFFIXES];
    int affix_count;
    
    // Reduplication
    RedupType redup_type;
    char redup_pattern[32];
    
    // Morphophonemic
    char morpho_changes[512];
    
    // Generated analysis strings
    char structure[512];                // e.g., "[gi-] + palit + [-an]"
    char gloss[512];                    // e.g., "was bought from"
    
    // Debugging
    char trace[16][256];
    int trace_count;
    
    float confidence_score;             // 0.0 - 1.0
    
    struct AnalysisResult* next;        // For multiple analyses
} AnalysisResult;

// Initialize the analyzer with data directory path
bool init_analyzer_v2(const char* data_dir);

// Analyze a word (main entry point)
// Returns linked list of all possible analyses
AnalysisResult* analyze_word_v2(const char* word, const char* language);

// Free a single analysis result
void free_analysis_result(AnalysisResult* result);

// Free linked list of analysis results
void free_analysis_results(AnalysisResult* results);

// Get number of analyses
int count_analyses(AnalysisResult* results);

// Print analysis result (for debugging)
void print_analysis_result(const AnalysisResult* result);

// Print all analyses
void print_all_analyses(const AnalysisResult* results);

// Cleanup the analyzer
void cleanup_analyzer_v2(void);

// =====================================================
// Legacy API (backward compatibility)
// =====================================================

typedef struct {
    bool valid;
    char word[MAX_WORD_LEN];
    char prefix[64];
    char root[128];
    char suffix[64];
    Language language;
    char trace[10][256];
    int trace_count;
} LegacyAnalysisResult;

// Legacy initialization (uses old format)
void init_analyzer(const char* prefix_data, 
                   const char* suffix_data,
                   const char* bisaya_data, 
                   const char* tagalog_data,
                   const char* shared_data);

// Legacy analysis function
void analyze_word(const char* word, LegacyAnalysisResult* result);

// Legacy cleanup
void cleanup_analyzer(void);

#endif