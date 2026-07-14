#ifndef __FIBER_JOB_SYSTEM_REGISTERS_H__
#define __FIBER_JOB_SYSTEM_REGISTERS_H__

    #include <stdint.h>
    #include <stddef.h>

    // x64 SysV store regs
    struct _x64_SysV_store_regs{
        uintptr_t rbx; // 0
        uintptr_t r12; // 8
        uintptr_t r13; // 16
        uintptr_t r14; // 24
        uintptr_t r15; // 32
    }__attribute__((packed));
    typedef struct _x64_SysV_store_regs _x64_SysV_store_regs;


    // x86 cdecl store regs
    struct _x86_Cdecl_store_regs{
        uintptr_t ebx; // 0
        uintptr_t esi; // 4
        uintptr_t edi; // 8
        uintptr_t zro; // zero padding for alignment
    }__attribute__((packed));
    typedef struct _x86_Cdecl_store_regs _x86_Cdecl_store_regs;

    union _fiber_registers{
        _x64_SysV_store_regs    _x64_regs;
        _x86_Cdecl_store_regs   _x86_regs;
    };
    typedef union _fiber_registers _fiber_registers;

#endif 