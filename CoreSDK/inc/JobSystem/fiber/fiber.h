#include "fiber_context.h"
/**
 * @file fiber.h 
 * 
 *
 * @brief All basic functionality for initialize fibers , reset , finilized and 
 * switch fiber . As well here fibers are handle per thread using tls . 
 * 
 */
#ifndef __FIBER_JOB_SYSTEM_FIBER_H__
#define __FIBER_JOB_SYSTEM_FIBER_H__
#ifdef __cplusplus
extern "C" {
#endif 
    
    struct fiber;
    typedef void (*fiber_proc)(struct fiber* fib , void* arg);

    /**
     * @brief FIBER Possible states 
     * 
     */
    enum fiber_state{
        FIBER_READY     = 1,
        FIBER_RUNNING   = 2,
        FIBER_FINISHED  = 3
    };
    typedef enum fiber_state fiber_state;
    
    /**
     * @brief Fiber struct has all information for them , like fiber stack context etc.
     * @var id Is the guid of the fiber , do not change it .
     * @var ctx The Fiber Stack Context .
     * @var prc The Fiber Procedure .
     * @var stt The Fiber State .
     * @var arg The Argument for the fiber procedure . This may change on the future 
     * to support local variables .
     */
    struct fiber {
        uint32_t        id;
        _fiber_context  ctx;
        fiber_proc      prc;
        fiber_state     stt;
        void*           arg;
    };
    typedef struct fiber fiber;

    /**
     * @brief Initialize a fiber . Fiber state set to READY .
     * @note First Time is calling the thread fiber initialization , so thread fiber
     * stack context can be saved .
     * @param fib Fiber pointer
     * @param stack_size Fiber Stack Size
     * @param routine Fiber routine 
     * @param arg Fiber routine argument
     * @return int 
     * Returns :
     *      1. __FIBER_ERR_OK : If everything completed
     *      2. __FIBER_ERR_BARG if routine or fiber is NULL .
     *      3. __FIBER_ERR_BARG if stack size is zero . 
     *      4. __FIBER_ERR_TSSS if stack size is less than 1KB 
     *      5. __FIBER_ERR_PSUN if we can't find the os page size . 
     *      6. __FIBER_ERR_PMFL if paging request's failed .
     */
    SDK_API int fiber_init(fiber* fib , uint32_t stack_size  , fiber_proc routine , void* arg);
    
    /**
     * @brief Reset's the Fiber stack context to the initial one at creation . 
     * Ofcourse the difference is that makes only stack pointer and instruction 
     * pointer reset like a new fiber . This can be used for reusing the same fibers .
     * In order to happen Fiber should not be the current RUNNING FIBER , it's state 
     * should be either FINISHED or READY and transits to READY state after the reset call.
     * 
     * @param fib The fiber pointer to reset .
     * @param routine The routine that the fiber will start executing .
     * @param arg Argument to the fiber routine .
     * @return int 
     * Returns : 
     *      1. __FIBER_ERR_OK   : If everything completed .
     *      2. __FIBER_ERR_BARG : If routine or fib are null .
     *      3. __FIBER_ERR_INVLD: If fib state is not FINISHED or READY
     *      4. __FIBER_ERR_FNIN : If fib is not previusly initialized .
     *      5. __FIBER_ERR_TSSS : If fib stack has errors , like be too small .
     */
    SDK_API int fiber_reset(fiber* fib , fiber_proc routine , void* arg);

    /**
     * @brief Finilized the fiber , like un mapp os pages of the stack and 
     * free space .
     * 
     * @param fib The fiber pointer to finilize .
     * @return int 
     * Returns:
     *      1. __FIBER_ERR_OK   : If everything is completed.
     *      2. __FIBER_ERR_BARG : If fib is NULL 
     *      3. __FIBER_ERR_INVLD: If fib state is RUNNING 
     *      4. __FIBER_ERR_PMFL : If paging system calls failed to unmap pages . 
     */
    SDK_API int fiber_fini(fiber* fib );

    /**
     * @brief Use it to switch stack context to the fiber stack context and resume 
     * this specific fiber execution . Changes the state from READY to RUNNING but
     * the state should always be always READY . If is RUNNING  or basicly the thread 
     * current fiber is the same as target then the operation 
     * is discarded and returns success without actual stack context switching . 
     * @note If target is NULL then we switching back to thread stack context .
     * @warning One fiber context must run only to one thread at a time otherwise 
     * the behavior will be undefined . 
     * 
     * @param target The fiber to switch to .
     * @return int 
     * RETURNS :
     *      1. __FIBER_ERR_OK   : If everything completed.
     *      2. __FIBER_ERR_FINI : If the fiber is not initialized . 
     */
    SDK_API int fiber_switch(fiber* target);

    /**
     * @brief Same as fiber_switch(NULL) and can be used for switching 
     * back to thread stack context .
     * 
     */
    #define fiber_yield() fiber_switch(NULL)

    /**
     * @brief Returns Thread current fiber pointer .
     * 
     * @return fiber* Thread current fiber pointer .
     */
    SDK_API fiber* fiber_current(void);

    /**
     * @brief Returns the fiber id .
     * 
     * @param fib The fiber to get the id from . 
     * @return uint32_t 0 If fiber is not initialized or fib is NULL and a number 
     * from 1 to MAX(U32) as a guid , for max number of MAX(u32) fiber init calls .
     */
    SDK_API uint32_t fiber_get_id(fiber* fib);

    /**
     * @brief Returns the fiber state .
     * 
     * @param fib The fiber to get the state from . 
     * @return fiber_state FIBER_READY , FIBER_RUNNING , FIBER_FINISHED or 0 if 
     * fib is NULL or not initialized .
     */
    SDK_API fiber_state fiber_get_state(fiber* fib);

#ifdef __cplusplus
}
#endif 
#endif