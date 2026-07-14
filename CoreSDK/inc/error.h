#ifndef __FIBER_JOB_SYSTEM_ERROR_H__
#define __FIBER_JOB_SYSTEM_ERROR_H__
    #include <stdint.h>
    #include <stddef.h>

    #define __FIBER_ERR_OK      ((uint32_t)0x00000000)
    #define __FIBER_ERR_GENERAL ((uint32_t)0x00000001) // Unknown Error
    #define __FIBER_ERR_PSUN    ((uint32_t)0x00000002) // Page Size Unknown or couldn't find
    #define __FIBER_ERR_BARG    ((uint32_t)0x00000003) // Bad Argument 
    #define __FIBER_ERR_PMFL    ((uint32_t)0x00000004) // Paging Mapp Failed 
    #define __FIBER_ERR_TSSS    ((uint32_t)0x00000005) // Too Small Stack Size
    #define __FIBER_ERR_FNIN    ((uint32_t)0x00000006) // Fiber Not Initiallized
    #define __FIBER_ERR_INVLD   ((uint32_t)0x00000007) // Invalid operation 
#endif 