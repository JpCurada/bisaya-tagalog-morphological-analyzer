#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

typedef struct Entry {
    char* key;
    char* value; // JSON string or simple value
    struct Entry* next;
} Entry;

typedef struct HashTable {
    Entry** buckets;
    unsigned int size;
    unsigned int count;
} HashTable;

// Create a new hash table
HashTable* ht_create(unsigned int size);

// Insert a key-value pair
void ht_insert(HashTable* table, const char* key, const char* value);

// Lookup a value by key (returns NULL if not found)
char* ht_lookup(HashTable* table, const char* key);

// Check if key exists
bool ht_exists(HashTable* table, const char* key);

// Destroy the hash table and free memory
void ht_destroy(HashTable* table);

#endif