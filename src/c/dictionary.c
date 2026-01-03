#include "dictionary.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DJB2 Hash Function
static unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

HashTable* ht_create(unsigned int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->size = size;
    table->count = 0;
    table->buckets = (Entry**)calloc(size, sizeof(Entry*));
    
    if (!table->buckets) {
        free(table);
        return NULL;
    }
    
    return table;
}

void ht_insert(HashTable* table, const char* key, const char* value) {
    if (!table || !key) return;

    unsigned long index = hash_function(key) % table->size;
    Entry* current = table->buckets[index];

    // Check availability (update if exists)
    while (current) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = my_strdup(value);
            return;
        }
        current = current->next;
    }

    // Insert new entry
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    if (!new_entry) return;

    new_entry->key = my_strdup(key);
    new_entry->value = my_strdup(value);
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->count++;
}

char* ht_lookup(HashTable* table, const char* key) {
    if (!table || !key) return NULL;

    unsigned long index = hash_function(key) % table->size;
    Entry* current = table->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

bool ht_exists(HashTable* table, const char* key) {
    return ht_lookup(table, key) != NULL;
}

void ht_destroy(HashTable* table) {
    if (!table) return;

    for (unsigned int i = 0; i < table->size; i++) {
        Entry* current = table->buckets[i];
        while (current) {
            Entry* next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(table->buckets);
    free(table);
}