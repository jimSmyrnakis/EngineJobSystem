#include <JobSystem/fiber/fiber.h>
#include <stdlib.h> 
#include <JobSystem/fiber/arch.h> 
#include <sdk_macrosz.h>
    // fiber of each thread 
    static _Thread_local fiber          thread_fiber;
    static _Thread_local uint8_t        thread_has_init = 0;

    // current fiber that runs on this fiber
    static _Thread_local fiber*         current_fiber = NULL;

    // GUID - TODO : make atomic ops 
    static _Atomic uint32_t s_id = 0;
    
    static void _trap_function(void){
        
        FIBER_ASSERT(0 , "Fiber out of stack bounds !!!\n");
    } 

    static inline void  try_init_thread_fiber(){
        if (thread_has_init == 0){
            if (_arch_layer_is_init() != __FIBER_ERR_OK)
            _arch_layer_init();
            thread_fiber.id = ++s_id;
            thread_fiber.ctx.sig =  _FIBER_CONTEXT_SIG ;
            thread_fiber.stt = FIBER_RUNNING;
            thread_has_init = 1;
            
            FIBER_ASSERT( current_fiber == NULL , 
            "Somehow current fiber is set :( .");
            current_fiber = &thread_fiber;
        }
        
    }

    static void fiber_entry(void){
        FIBER_ASSERT(
            (current_fiber != NULL) && (current_fiber->prc != NULL) , 
            "Fiber or fiber procedure are not set .");
        
        current_fiber->prc(current_fiber , current_fiber->arg);
        fiber* old = current_fiber;
        current_fiber = &thread_fiber;
        old->stt = FIBER_FINISHED;
        current_fiber->stt = FIBER_RUNNING;
        _fiber_context_switch(&old->ctx , &thread_fiber.ctx);
        FIBER_ASSERT(0,"fiber_entry returned unexpectedly.");
    }

    int fiber_init(fiber* fib , uint32_t stack_size , fiber_proc routine , void* arg){
        try_init_thread_fiber();
        

        if ( !routine || !fib )
            return __FIBER_ERR_BARG;

        int res =  _fiber_context_init(&fib->ctx , stack_size , fiber_entry, _trap_function);
        if (res == __FIBER_ERR_OK){
            fib->id = ++s_id;
            fib->prc = routine;
            fib->stt = FIBER_READY;
            fib->arg = arg;
        }

        return res;
    }

    int fiber_reset(fiber* fib , fiber_proc routine , void* arg){
        try_init_thread_fiber();

        
        if (!routine || !fib )
            return __FIBER_ERR_BARG;

        // should not running when reseting 
        if (current_fiber == fib ) 
            return __FIBER_ERR_INVLD;
        if (fib->stt != FIBER_FINISHED && fib->stt != FIBER_READY)
            return __FIBER_ERR_INVLD;

        fib->prc = routine;
        fib->arg = arg;
        
        int res = _fiber_context_reset(&fib->ctx , fiber_entry , _trap_function);
        if (res == __FIBER_ERR_OK){
            fib->stt = FIBER_READY;
        }

        return res;
    }
    
    int fiber_fini(fiber* fib ){
        try_init_thread_fiber();
        if (!fib)
            return __FIBER_ERR_BARG;
        if ( (fib == current_fiber) || (fib->stt == FIBER_RUNNING) )
            return __FIBER_ERR_INVLD;
        fib->stt = FIBER_FINISHED;
        fib->prc = NULL;
        fib->ctx._instr_ptr = 0;
        return _fiber_context_fini(&fib->ctx);
    }

    int fiber_switch(fiber* target){
        try_init_thread_fiber();

        FIBER_ASSERT(current_fiber && (current_fiber->stt == FIBER_RUNNING) , 
        "The current fiber should always be valid and running !" );

        if (target == NULL){
            // switch to thread fiber
            target = &thread_fiber;
        }

        
        fiber* old = current_fiber;

        FIBER_ASSERT(
        target->stt != FIBER_FINISHED,
        "Cannot switch to a finished fiber.");
        

        if ( (target == current_fiber) )
            return __FIBER_ERR_OK;
        FIBER_ASSERT(target->stt == FIBER_READY , "It should be READY state !");
            
        
        old->stt = FIBER_READY;
        target->stt = FIBER_RUNNING;
        current_fiber = target;
        return _fiber_context_switch(&old->ctx , &target->ctx);
        
    }


    fiber* fiber_current(void){
        return current_fiber;
    }

    uint32_t fiber_get_id(fiber* fib){
        if (fib == NULL)
            return 0;

        if (fib->ctx.sig != _FIBER_CONTEXT_SIG)
            return 0;

        return fib->id;
    }

    fiber_state fiber_get_state(fiber* fib){
        if (fib == NULL)
            return 0;

        if (fib->ctx.sig != _FIBER_CONTEXT_SIG)
            return 0;

        return fib->stt;
    }
