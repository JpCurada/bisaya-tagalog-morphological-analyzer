#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Generic queue implementation for BFS traversal.
 * Stores void pointers to allow any data type.
 */

typedef struct QueueNode {
    void* data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    size_t size;
} Queue;

// Create a new queue
Queue* queue_create(void);

// Enqueue an item (adds to rear)
void queue_enqueue(Queue* q, void* data);

// Dequeue an item (removes from front, returns data)
// Returns NULL if queue is empty
void* queue_dequeue(Queue* q);

// Peek at front item without removing
void* queue_peek(Queue* q);

// Check if queue is empty
bool queue_is_empty(Queue* q);

// Get queue size
size_t queue_size(Queue* q);

// Destroy queue (does NOT free the data pointers)
void queue_destroy(Queue* q);

// Destroy queue and free all data pointers
void queue_destroy_with_data(Queue* q);

#endif
