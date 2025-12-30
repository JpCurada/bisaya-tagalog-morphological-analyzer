#include "analyzer.h"
#include <stdio.h>
#include <string.h>

void print_result(const char* label, AnalysisResult* res) {
    printf("Test: %s\n", label);
    printf("Word: %s\n", res->word);
    printf("Valid: %s\n", res->valid ? "Yes" : "No");
    printf("Prefix: %s\n", res->prefix);
    printf("Root: %s\n", res->root);
    printf("Suffix: %s\n", res->suffix);
    printf("Language: %s\n", language_to_string(res->language));
    printf("Trace:\n");
    for (int i=0; i < res->trace_count; i++) {
        printf("  %s\n", res->trace[i]);
    }
    printf("--------------------------------\n");
}

int main() {
    // Dummy Data
    const char* prefix_data = 
        "nag-|Both|actor_focus_completed,verb\n"
        "mag-|Both|actor_focus_contemplated,verb";
    
    const char* suffix_data = 
        "-an|Both|locative_focus,verb,noun";
        
    const char* bisaya_data = 
        "kaon|Bisaya|verb\n"
        "balay|Bisaya|noun";
        
    const char* tagalog_data = 
        "kain|Tagalog|verb";
        
    const char* shared_data = 
        "luto|Both|verb";

    init_analyzer(prefix_data, suffix_data, bisaya_data, tagalog_data, shared_data);
    
    AnalysisResult res;
    
    // Test 1: Bisaya "nag-kaon"
    analyze_word("nag-kaon", &res);
    print_result("Bisaya: nag-kaon", &res);
    
    // Test 2: Tagalog "mag-kain" (Wait, mag-kain is valid? mag- is Both, kain is Tagalog. Compatible.)
    analyze_word("mag-kain", &res);
    print_result("Tagalog: mag-kain", &res);
    
    // Test 3: Shared "nag-luto"
    analyze_word("nag-luto", &res);
    print_result("Shared: nag-luto", &res);
    
    // Test 4: Invalid "nag-xyz"
    analyze_word("nag-xyz", &res);
    print_result("Invalid: nag-xyz", &res);
    
    cleanup_analyzer();
    return 0;
}
