#ifndef __FIBER_JOB_SYSTEM_QUEUE_H__
#define __FIBER_JOB_SYSTEM_QUEUE_H__


#ifdef __cplusplus
extern "C" {
#endif

    #include <stdatomic.h>
    #define ATOMIC(type) _Atomic type

    #include <stdint.h>
    #include <stddef.h>
    #include "../../error.h"
    #include "../../sdk_macrosz.h"
    struct lf_spmc_fz_queue{
        // lock free , single producer , multiple consumer , fixed size queue

        
        ATOMIC(uint32_t)  head;
        ATOMIC(uint32_t)  tail;

        uint32_t signature;
        uint8_t* buffer;
        uint16_t element_length;
        uint32_t capacity;

    };
    typedef struct lf_spmc_queue lock_free_queue;

 
    SDK_API lock_free_queue*  lf_queue_create(uint32_t capacity, uint16_t element_length);

    SDK_API void lf_queue_destroy(lock_free_queue* queue);
    
    SDK_API int lf_queue_push(lock_free_queue* queue, const void* element);

    SDK_API int lf_queue_pop(lock_free_queue* queue, void* element);

#ifdef __cplusplus
}
#endif
#endif 