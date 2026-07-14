
#include <JobSystem/fiber/fiber_context.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <JobSystem/fiber/arch.h> 
#include <os.h>
#ifdef __cplusplus
    extern "C" {
#endif 
   
        
        #define SIG _FIBER_CONTEXT_SIG
        // some common data
        static int page_size = 0;

        

        #define round_to_page_len(size , page_size) (((size) + (page_size) - 1) & ~((page_size) - 1))











        int _fiber_context_init(_fiber_context* ctx , uint32_t stack_size , void* entry_point , void* exit_point){
            if (page_size == 0)
                page_size = _aoshll_page_size();
            
            int res = __FIBER_ERR_OK;
            
            if (page_size <= 0)
                return __FIBER_ERR_PSUN;

            if ( !stack_size || !ctx || !entry_point || !exit_point)
                return __FIBER_ERR_BARG;

            if ( stack_size < 1024)
                return __FIBER_ERR_TSSS;

            // stack scheme with pages
            // ----- page 0 - Guard Page ----- ==> No write mode at least
            // ----- page 1 ... N Stack pages ----- ==> Always write , read mode
            // ----- page N+1 - Guard Page ----- ==> No write mode at least

            // total pages required for stack in bytes
            size_t min_page_len = round_to_page_len(stack_size , page_size);
            // total stack pages + 2 Guard pages
            size_t total_page_len = 2 * page_size + min_page_len;

            // init context
            ctx->_stack_size = min_page_len;
            

            
            // call mmap to create writable pages .
            // Stack base just point to the mmap 
            // result and should stay that way .
            // That address is the lowest where 
            // page guard 0 is .
            // check if mapping is succesfull 
            res = _aoshll_mmap(total_page_len , 
                _OS_PAGE_PROT_READ | _OS_PAGE_PROT_WRITE , &ctx->_stack_base);
            if (res != 0)
                    return __FIBER_ERR_PMFL;
            
            // set page guard 0
            ctx->_guard_page0 = 
            ctx->_stack_base;

            // set page guard N+1
            ctx->_guard_pageN = (void*) 
            (((uint8_t*)ctx->_stack_base) + total_page_len - page_size);

            // in x86-64 represent's the rsp register
            ctx->_stack_ptr = 
            (uintptr_t)ctx->_stack_base + page_size + min_page_len;
            

            // make guard pages have PROT_NONE so we can't access them
            if (_aoshll_mprotect(ctx->_guard_page0 , page_size , _OS_PAGE_PROT_NONE) != 0){
                //try free the pages
                _aoshll_unmmap(ctx->_stack_base , total_page_len);
                return __FIBER_ERR_PMFL;
            }
            if (_aoshll_mprotect(ctx->_guard_pageN , page_size , _OS_PAGE_PROT_NONE) != 0){
                //try free the pages
                _aoshll_unmmap(ctx->_stack_base , total_page_len);
                return __FIBER_ERR_PMFL;
            }
            
            
            res =  _arch_layer_init_stack(ctx , 
                ctx->_stack_ptr , 
                ctx->_stack_ptr - min_page_len, entry_point , exit_point);
            if (res != __FIBER_ERR_OK){
                //try free the pages
                _aoshll_unmmap(ctx->_stack_base , total_page_len);
                return res;
            }
            ctx->sig = SIG;
            return __FIBER_ERR_OK;
        }











        int _fiber_context_reset(_fiber_context* ctx , void* entry_point , void* exit_point){
            if (!ctx || !entry_point)
                return __FIBER_ERR_BARG;
            
            if (ctx->sig != SIG)
                return __FIBER_ERR_FNIN;

            return _arch_layer_init_stack(ctx , 
                ctx->_stack_ptr , 
                ctx->_stack_ptr - ctx->_stack_size, entry_point , exit_point);
        }









        
        int _fiber_context_fini(_fiber_context* ctx){

            if (!ctx)
                return __FIBER_ERR_BARG;
            if (ctx->sig != SIG)
                return __FIBER_ERR_FNIN;

            // total pages required for stack in bytes
            size_t min_page_len = round_to_page_len(ctx->_stack_size , page_size);
            // total stack pages + 2 Guard pages
            size_t total_page_len = 2 * page_size + min_page_len;
            int res = _aoshll_unmmap(ctx->_stack_base , total_page_len);
            if (res != 0)
                return __FIBER_ERR_PMFL;
            ctx->sig = 0;
            return __FIBER_ERR_OK;
        }









        int _fiber_context_switch(_fiber_context* this_fiber , _fiber_context* target_fiber){
            
            if (!this_fiber || !target_fiber)
                return __FIBER_ERR_BARG;

            if (target_fiber->sig != SIG)
                return __FIBER_ERR_FNIN;
            
            if (this_fiber->sig != SIG)
                return __FIBER_ERR_FNIN;

            _fiber_switch_direct(this_fiber , target_fiber);


            return 0;

        }





#ifdef __cplusplus
    }
#endif 