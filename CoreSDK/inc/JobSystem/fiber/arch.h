#ifndef __FIBER_JOB_SYSTEM_ARCH_H__
#define __FIBER_JOB_SYSTEM_ARCH_H__

#ifdef __cplusplus
extern "C" {
#endif  
    #include "../../error.h"
    #include <stdint.h> 
    #include <stddef.h>
    #include "../../sdk_macrosz.h"  
    struct _fiber_context;

    struct _arch_layer{

        int (*init_stack)(
            struct _fiber_context* ctx  , 
            uintptr_t high              ,
            uintptr_t low               ,
            void* entry                 ,
            void* exit_point         
        );



        
    };
    typedef struct _arch_layer _arch_layer;
    extern SDK_API _arch_layer runtime_layer;

    SDK_API void _arch_layer_init(void);

    SDK_API int _arch_layer_is_init(void);

    #define _arch_layer_init_stack(ctx , high , low , entry_point , exit_point)\
    _arch_layer_is_init() == __FIBER_ERR_OK ? \
    ( (int)runtime_layer.init_stack(ctx , high , low , entry_point , exit_point ) ) : \
    (int)__FIBER_ERR_FNIN 

 
    

#ifdef __cplusplus
}
#endif 
#endif 