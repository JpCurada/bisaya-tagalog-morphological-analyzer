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
