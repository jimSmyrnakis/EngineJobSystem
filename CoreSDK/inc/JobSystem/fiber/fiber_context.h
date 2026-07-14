#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "../../error.h"
#include "../../sdk_macrosz.h"
#include "registers.h"

/**
 * @file fiber_context.h 
 * @warning  Every function with _funName format is system function and should not 
 * be called from outside this library .
 * @brief Here is all functionality for the core fiber stack logic . We can find initialization
 * , finalization , reset and switch fiber stack logic that makes possible to handle fibers . What
 * we can't find here is thread context saving so fibers can come back to its running 
 * thread context but here is the functionality to make that possible (as fiber.h does) .
 * @author Dimitris Smyrnakis
 * 
 */
#ifndef __FIBER_JOB_SYSTEM_FIBER_CONTEXT_H__
#define __FIBER_JOB_SYSTEM_FIBER_CONTEXT_H__
    #ifdef __cplusplus
    extern "C" {
    #endif 

    #define _FIBER_CONTEXT_SIG 0xABCDEF12
    
    /**
     * @brief This struct is system struct and change it by "hand" can create
     * Undefined behaviors . 
     * @struct _fiber_context Contains stack snapshot , store registers per architecture 
     * and info important to the fiber stack context .  
     */
    struct _fiber_context{
        // The instruction pointer 
        uintptr_t           _instr_ptr;
        // stack frame pointrr  
        uintptr_t           _frame_ptr;
        // stack pointer 
        uintptr_t           _stack_ptr;
        // store registers (Followed by the ABI , SysV for x64 linux)
        _fiber_registers    _registers;
        // stack base address used for mmap or similar system calls
        // and points to the first page of stack pages (Guard pages too)
        void*               _stack_base;
        // Total size in bytes of stack pages (Guard pages too)
        uintptr_t           _stack_size;
        
        // this page addresses are the Guard pages that protects
        // access on stack .
        void* _guard_page0; // first page guard 
        void* _guard_pageN; // last page guard 

        // Used to signal the functions use this struct that it has
        // first initialized . Programmer protection .
        uint32_t sig ; 

    }__attribute__((packed));
    typedef struct _fiber_context _fiber_context;

    

    /**
     * @brief Initialize a fiber context , creating stack pages etc. to ensure 
     * right behavior . 
     * 
     * @param ctx The fiber context 
     * @param stack_size The size of the stack in bytes
     * @param entry_point Pointer to procedure that the fiber will start executing .
     * @param exit_point Pointer to Procedure that the fiber will go after all stack unrolled
     * @return int 
     * 1. __FIBER_ERR_OK if everything is completed .
     * 2. __FIBER_ERR_BARG if ctx or stack size or entry_point or exit_point are zero/NULL 
     * 3. __FIBER_ERR_TSSS if stack size is less than 1KB 
     * 4. __FIBER_ERR_PSUN if we can't find the os page size 
     * 5. __FIBER_ERR_PMFL if paging request's failed .
     */
    SDK_API int _fiber_context_init(
        _fiber_context* ctx , 
        uint32_t stack_size , 
        void* entry_point   , 
        void* exit_point
    );

    /**
     * @brief Initialization allocates pages and set their protection properties , reset
     * reset the behavior of the fiber like is never used . 
     * @warning Carefull DO NOT call this function on the fiber is running , always reset
     * from when other fiber runs . Prefer when you running on thread stack context . 
     * @param ctx The fiber context we want to reset
     * @param entry_point The Entry point procedure / executable virtual address 
     * @param exit_point The Exit point procedure / executable virtual address . Used
     * when the fiber stack is unrolled as a return address to the exit point . 
     * @return int Returns :
     * 1. __FIBER_ERR_BARG if any of the parameters is NULL .
     * 2. __FIBER_ERR_FNIN if context is not initialized before .
     * 3. __FIBER_ERR_OK   if everything completed without errors .
     * 4. __FIBER_ERR_TSSS if stack has errors , like be too small . 
     */
    SDK_API int _fiber_context_reset(_fiber_context* ctx , void* entry_point , void* exit_point);

    /**
     * @brief Finilize the fiber context , free's pages etc.
     * 
     * @param ctx The fiber Context we want to finilize
     * @return int 
     * 1. __FIBER_ERR_OK if finilization happen succesfully .
     * 2. __FIBER_ERR_BARG if ctx is null .
     * 3. __FIBER_ERR_PMFL if unmapping failed . 
     */ 
    SDK_API int _fiber_context_fini(_fiber_context* ctx);

    /**
     * @brief Switch Fibers context on our thread stack context . Saves
     * in this_fiber the current context of stack and store registers and switch/load
     * the stack context to target_fiber stack context and store regs . 
     * @warning The this_fiber should be the current fiber running on the thread that called .
     * Otherwise this_fiber will change its stack snapshot to the current running stack context .
     * @param this_fiber The fiber we currently are .
     * @param target_fiber The fiber we want to switch to .
     * @return int 
     * 1. __FIBER_ERR_OK if everything happened succefully . 
     * 2. __FIBER_ERR_FINI if no one of the fibers is not initialized
     * 3. __FIBER_ERR_BARG if this_fiber or target fiber are NULL .
     */
    SDK_API int _fiber_context_switch(
        _fiber_context* this_fiber , 
        _fiber_context* target_fiber
    );

    
    
    /**
     * @brief This function/procedure is implemented in assembly by the current 
     * architecture . Please do not calling it .
     * @param this_fiber 
     * @param target_fiber 
     */
    extern SDK_API void _fiber_switch_direct(
        _fiber_context* this_fiber , 
        _fiber_context* target_fiber 
    );

    
    #ifdef __cplusplus
    }
    #endif 

#endif  