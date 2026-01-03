#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include <stddef.h>

/**
 * String-based HashSet for tracking visited states in BFS.
 * Uses open hashing (chaining) for collision resolution.
 */

typedef struct HashSetNode {
    char* key;
    struct HashSetNode* next;
} HashSetNode;

typedef struct HashSet {
    HashSetNode** buckets;
    size_t capacity;
    size_t size;
} HashSet;

// Create a new hashset with given capacity
HashSet* hashset_create(size_t capacity);

// Add a string to the set
// Returns true if added, false if already exists
bool hashset_add(HashSet* set, const char* key);

// Check if string exists in set
bool hashset_contains(HashSet* set, const char* key);

// Remove a string from set
// Returns true if removed, false if not found
bool hashset_remove(HashSet* set, const char* key);

// Get number of elements in set
size_t hashset_size(HashSet* set);

// Clear all elements from set
void hashset_clear(HashSet* set);

// Destroy the hashset and free all memory
void hashset_destroy(HashSet* set);

#endif
