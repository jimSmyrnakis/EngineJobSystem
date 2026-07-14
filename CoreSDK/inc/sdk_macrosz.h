#ifndef __SDK_MACROSZ_H__
#define __SDK_MACROSZ_H__


    #if defined(DEBUG) || defined(_DEBUG) || defined(_DEBUG_)
            
        #if defined(__i386__) || defined(__x86_64__)
            #define DEBUG_BREAK() __asm__ volatile("int3")

        #elif defined(__aarch64__)
            #define DEBUG_BREAK() __asm__ volatile("brk #0")

        #else
            #include <signal.h>
            #define DEBUG_BREAK() raise(SIGTRAP)
        #endif

    #else
        #define DEBUG_BREAK() exit(1)
    #endif

    #define FIBER_ASSERT(cond , msg)  \
    if (!(cond)) {\
        printf("On %s:%d ==> Fatal Error : %s" , __FILE__ , __LINE__ , (msg));\
        DEBUG_BREAK();\
    }

    
        #if defined(_SDK_WINDOWS_)

            #ifdef _SDK_BUILD
                #define SDK_API __declspec(dllexport)
            #else
                #define SDK_API __declspec(dllimport)
            #endif

        #else

            #define SDK_API __attribute__((visibility("default")))

        #endif


#endif 