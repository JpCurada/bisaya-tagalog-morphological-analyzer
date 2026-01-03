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

// Trim leading and trailing whitespace in-place
void trim(char* str);

// Check if character is a vowel (a, e, i, o, u)
bool is_vowel(char c);

// Check if character is a consonant
bool is_consonant(char c);

// Insert a character at a specific position
// Returns new allocated string (caller must free)
char* insert_char_at(const char* str, char c, int pos);

// Remove a character at a specific position
// Returns new allocated string (caller must free)
char* remove_char_at(const char* str, int pos);

// Convert string to lowercase in-place
void to_lowercase(char* str);

// Create a lowercase copy of string (caller must free)
char* to_lowercase_copy(const char* str);

// Remove hyphens from string in-place
void remove_hyphens(char* str);

// Normalize word (lowercase, remove hyphens)
void normalize_word(const char* input, char* output, size_t size);

#endif