#include "hashset.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

// DJB2 hash function
static unsigned long hash_string(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

HashSet* hashset_create(size_t capacity) {
    if (capacity == 0) capacity = 64;
    
    HashSet* set = (HashSet*)malloc(sizeof(HashSet));
    if (!set) return NULL;
    
    set->buckets = (HashSetNode**)calloc(capacity, sizeof(HashSetNode*));
    if (!set->buckets) {
        free(set);
        return NULL;
    }
    
    set->capacity = capacity;
    set->size = 0;
    
    return set;
}

bool hashset_add(HashSet* set, const char* key) {
    if (!set || !key) return false;
    
    // Check if already exists
    if (hashset_contains(set, key)) {
        return false;
    }
    
    unsigned long index = hash_string(key) % set->capacity;
    
    HashSetNode* node = (HashSetNode*)malloc(sizeof(HashSetNode));
    if (!node) return false;
    
    node->key = my_strdup(key);
    if (!node->key) {
        free(node);
        return false;
    }
    
    node->next = set->buckets[index];
    set->buckets[index] = node;
    set->size++;
    
    return true;
}

bool hashset_contains(HashSet* set, const char* key) {
    if (!set || !key) return false;
    
    unsigned long index = hash_string(key) % set->capacity;
    
    HashSetNode* current = set->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

bool hashset_remove(HashSet* set, const char* key) {
    if (!set || !key) return false;
    
    unsigned long index = hash_string(key) % set->capacity;
    
    HashSetNode* current = set->buckets[index];
    HashSetNode* prev = NULL;
    
    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                set->buckets[index] = current->next;
            }
            free(current->key);
            free(current);
            set->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

size_t hashset_size(HashSet* set) {
    if (!set) return 0;
    return set->size;
}

void hashset_clear(HashSet* set) {
    if (!set) return;
    
    for (size_t i = 0; i < set->capacity; i++) {
        HashSetNode* current = set->buckets[i];
        while (current) {
            HashSetNode* next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
        set->buckets[i] = NULL;
    }
    
    set->size = 0;
}

void hashset_destroy(HashSet* set) {
    if (!set) return;
    
    hashset_clear(set);
    free(set->buckets);
    free(set);
}
