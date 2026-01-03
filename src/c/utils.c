#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void safe_strcpy(char* dest, const char* src, size_t size) {
    if (size == 0) return;
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

bool starts_with(const char* str, const char* prefix) {
    if (!str || !prefix) return false;
    size_t len_pre = strlen(prefix);
    size_t len_str = strlen(str);
    if (len_pre > len_str) return false;
    return strncmp(str, prefix, len_pre) == 0;
}

bool ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    size_t len_suf = strlen(suffix);
    size_t len_str = strlen(str);
    if (len_suf > len_str) return false;
    return strcmp(str + len_str - len_suf, suffix) == 0;
}

char* strip_prefix_copy(const char* str, const char* prefix) {
    if (!starts_with(str, prefix)) return NULL;
    size_t len_pre = strlen(prefix);
    return my_strdup(str + len_pre);
}

char* strip_suffix_copy(const char* str, const char* suffix) {
    if (!ends_with(str, suffix)) return NULL;
    size_t len_suf = strlen(suffix);
    size_t len_str = strlen(str);
    size_t new_len = len_str - len_suf;
    
    char* new_str = (char*)malloc(new_len + 1);
    if (new_str) {
        strncpy(new_str, str, new_len);
        new_str[new_len] = '\0';
    }
    return new_str;
}

char* my_strdup(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* copy = (char*)malloc(len + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

void trim(char* str) {
    if (!str) return;
    
    // Trim leading whitespace
    char* start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }
    
    // Move string to beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    // Trim trailing whitespace
    size_t len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || 
                       str[len-1] == '\n' || str[len-1] == '\r')) {
        str[--len] = '\0';
    }
}

bool is_vowel(char c) {
    char lower = (c >= 'A' && c <= 'Z') ? c + 32 : c;
    return (lower == 'a' || lower == 'e' || lower == 'i' || 
            lower == 'o' || lower == 'u');
}

bool is_consonant(char c) {
    char lower = (c >= 'A' && c <= 'Z') ? c + 32 : c;
    return (lower >= 'a' && lower <= 'z' && !is_vowel(c));
}

char* insert_char_at(const char* str, char c, int pos) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    if (pos < 0 || pos > (int)len) return NULL;
    
    char* result = (char*)malloc(len + 2);  // +1 for new char, +1 for null
    if (!result) return NULL;
    
    // Copy part before insertion point
    memcpy(result, str, pos);
    
    // Insert the character
    result[pos] = c;
    
    // Copy part after insertion point
    strcpy(result + pos + 1, str + pos);
    
    return result;
}

char* remove_char_at(const char* str, int pos) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    if (pos < 0 || pos >= (int)len) return NULL;
    
    char* result = (char*)malloc(len);  // len - 1 + 1 for null
    if (!result) return NULL;
    
    // Copy part before removal point
    memcpy(result, str, pos);
    
    // Copy part after removal point
    strcpy(result + pos, str + pos + 1);
    
    return result;
}

void to_lowercase(char* str) {
    if (!str) return;
    
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
}

char* to_lowercase_copy(const char* str) {
    if (!str) return NULL;
    
    char* copy = my_strdup(str);
    if (copy) {
        to_lowercase(copy);
    }
    return copy;
}

void remove_hyphens(char* str) {
    if (!str) return;
    
    char* read = str;
    char* write = str;
    
    while (*read) {
        if (*read != '-') {
            *write++ = *read;
        }
        read++;
    }
    *write = '\0';
}

void normalize_word(const char* input, char* output, size_t size) {
    if (!input || !output || size == 0) return;
    
    safe_strcpy(output, input, size);
    to_lowercase(output);
    remove_hyphens(output);
}