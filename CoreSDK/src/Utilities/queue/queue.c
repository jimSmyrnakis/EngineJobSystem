#include <Utilities/queue/queue.h>
#include <memory.h>
#include <stdlib.h>
#define SIGNATURE 0xDEADBEEF

static uint32_t next_power_of_two(uint32_t x)
{
    if (x <= 1)
        return 1;

    x--;

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    return x + 1;
}

lock_free_queue* lf_queue_create(uint32_t capacity, uint16_t element_length){
    if (capacity == 0 || element_length == 0) {
        return NULL;
    }
    // force capacity to the closest up power of 2 (this ensures counters tail overflow
    // are not a bug of the code )
    capacity = next_power_of_two(capacity);

    lock_free_queue* queue = (lock_free_queue*)malloc(sizeof(lock_free_queue));
    if (!queue) 
        return NULL;

    queue->buffer = (uint8_t*)malloc(capacity * element_length);
    if (!queue->buffer) {
        free(queue);
        return NULL;
    }

    queue->capacity = capacity;
    queue->element_length = element_length;
    queue->head = 0;
    queue->tail = 0;
    queue->signature = SIGNATURE;

    return queue;
}

void lf_queue_destroy(lock_free_queue* queue){
    if (queue){
        free(queue->buffer);
        queue->signature = 0;
        free(queue);
    }
}





int lf_queue_push(lock_free_queue* queue, const void* element){
    

    if (queue->signature != SIGNATURE) {
        return -1; // unitialized queue
    }

    uint32_t tail = atomic_fetch_add(&queue->tail, 1);
    uint32_t head = atomic_load(&queue->head);
    
    int64_t size = tail - head;
    if (size >= queue->capacity) {
        return -2; // queue is full
    }

    
    uint32_t index = tail % queue->capacity;
    memcpy(queue->buffer + 
        index * queue->element_length, 
        element, queue->element_length);

    return 0;
}

int lf_queue_pop(lock_free_queue* queue, void* element){
    if (queue->signature != SIGNATURE) {
        return -1; // unitialized queue
    }

    uint32_t head = atomic_fetch_add(&queue->head, 1);
    uint32_t tail = atomic_load(&queue->tail);
    
    int64_t size = tail - head;
    if (size <= 0) {
        return -2; // queue is empty
    }

    
    uint32_t index = head % queue->capacity;
    memcpy(element, queue->buffer +
        index * queue->element_length, queue->element_length);

    return 0;
}