#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>
#include "morphology.h"

#define MAX_WORD_LEN 256
#define MAX_TRACE_LEN 1024

typedef struct {
    bool valid;
    char word[MAX_WORD_LEN];
    char prefix[64];
    char root[128];
    char suffix[64];
    Language language;
    char trace[10][256];
    int trace_count;
} AnalysisResult;

// Initialize the analyzer with raw data strings
// Strings are newline-delimited: "key|val\nkey2|val2..."
void init_analyzer(const char* prefix_data, 
                   const char* suffix_data,
                   const char* bisaya_data, 
                   const char* tagalog_data,
                   const char* shared_data);

// Analyze a single word
void analyze_word(const char* word, AnalysisResult* result);

// Cleanup memory
void cleanup_analyzer();

#endif
