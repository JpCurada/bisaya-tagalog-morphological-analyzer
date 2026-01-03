#include "queue.h"
#include <stdlib.h>

Queue* queue_create(void) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) return NULL;
    
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    
    return q;
}

void queue_enqueue(Queue* q, void* data) {
    if (!q) return;
    
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) return;
    
    node->data = data;
    node->next = NULL;
    
    if (q->rear == NULL) {
        // Empty queue
        q->front = node;
        q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    
    q->size++;
}

void* queue_dequeue(Queue* q) {
    if (!q || q->front == NULL) return NULL;
    
    QueueNode* temp = q->front;
    void* data = temp->data;
    
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    
    return data;
}

void* queue_peek(Queue* q) {
    if (!q || q->front == NULL) return NULL;
    return q->front->data;
}

bool queue_is_empty(Queue* q) {
    return (q == NULL || q->front == NULL);
}

size_t queue_size(Queue* q) {
    if (!q) return 0;
    return q->size;
}

void queue_destroy(Queue* q) {
    if (!q) return;
    
    QueueNode* current = q->front;
    while (current) {
        QueueNode* next = current->next;
        free(current);
        current = next;
    }
    
    free(q);
}

void queue_destroy_with_data(Queue* q) {
    if (!q) return;
    
    QueueNode* current = q->front;
    while (current) {
        QueueNode* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    
    free(q);
}
