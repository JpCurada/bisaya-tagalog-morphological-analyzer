#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>
#include "morphology.h"

#define MAX_WORD_LEN 256
#define MAX_TRACE_LEN 1024

#ifdef _WIN32
    #define ANALYZER_EXPORT __declspec(dllexport)
#else
    #define ANALYZER_EXPORT
#endif

typedef struct {
    bool valid;
    char word[MAX_WORD_LEN];
    char prefix[64];
    char prefix_info[256];
    char root[128];
    char root_pos[64];
    char root_definition[1024];
    char suffix[64];
    char suffix_info[256];
    Language language;
    char trace[10][256];
    int trace_count;
} AnalysisResult;

// Initialize the analyzer with raw data strings
// Strings are newline-delimited: "key|val\nkey2|val2..."
ANALYZER_EXPORT void init_analyzer(const char* affix_json_content,
                   const char* bisaya_data, const char* tagalog_data,
                   const char* hiligaynon_data,
                   const char* shared_data);

// Analyze a single word
ANALYZER_EXPORT void analyze_word(const char* word, AnalysisResult* result);

// Cleanup memory
ANALYZER_EXPORT void cleanup_analyzer();

#endif
