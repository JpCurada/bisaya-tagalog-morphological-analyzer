#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

// Safe string copy
void safe_strcpy(char* dest, const char* src, size_t size);

// Check if string starts with prefix
bool starts_with(const char* str, const char* prefix);

// Check if string ends with suffix
bool ends_with(const char* str, const char* suffix);

// Create a new string with prefix removed (caller must free)
char* strip_prefix_copy(const char* str, const char* prefix);

// Create a new string with suffix removed (caller must free)
char* strip_suffix_copy(const char* str, const char* suffix);

// Duplicate string (portable version)
char* my_strdup(const char* str);

#endif
