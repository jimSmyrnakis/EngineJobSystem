
#include <JobSystem/fiber/arch.h> 
#include <JobSystem/fiber/fiber_context.h> 
_arch_layer runtime_layer; 
static int is_init = 0; 
 


int _x64_SysV_init_stack(_fiber_context* ctx , uintptr_t high , uintptr_t low , void* entry , void* exit_point){
    if (!ctx || !entry || (high <= low)  )
        return __FIBER_ERR_BARG;

    if (((high - 8) <= low ))
        return __FIBER_ERR_TSSS;


    ctx->_instr_ptr = (uintptr_t)entry;
    ctx->_stack_ptr = high - 8;
    *((uintptr_t *)ctx->_stack_ptr) = (uintptr_t)exit_point;

    ctx->_frame_ptr = 0;
    ctx->sig = _FIBER_CONTEXT_SIG;
    return __FIBER_ERR_OK;
}

int _x86_cdecl_init_stack(_fiber_context* ctx , uintptr_t high , uintptr_t low , void* entry , void* exit_point){
    if (!ctx || !entry || (high <= low)  )
        return __FIBER_ERR_BARG;

    if (((high - 4) <= low ))
        return __FIBER_ERR_TSSS;

    if (ctx->sig != _FIBER_CONTEXT_SIG)
        return __FIBER_ERR_FNIN;

    ctx->_instr_ptr = (uintptr_t)entry;
    ctx->_stack_ptr = high - 4;
    *((uintptr_t *)ctx->_stack_ptr) = (uintptr_t)exit_point;

    ctx->_frame_ptr = 0;

    return __FIBER_ERR_OK;
}


void _arch_layer_init(void){
    if (is_init == 0){
        #if defined(__x86_64__)
            runtime_layer.init_stack = _x64_SysV_init_stack;  
        #elif defined(__i386__)
            runtime_layer.init_stack = _x86_cdecl_init_stack; 
        #else 
            #error Not supported Architecture     
        #endif 
    }
    is_init = 1;
}

int _arch_layer_is_init(void){
    return is_init ? __FIBER_ERR_OK : __FIBER_ERR_FNIN;
}