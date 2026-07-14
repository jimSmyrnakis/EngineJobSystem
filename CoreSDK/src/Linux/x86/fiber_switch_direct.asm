bits 32
section .text

global _fiber_switch_direct
_fiber_switch_direct:
    push ebp
    mov ebp , esp

    
    ; save previus ebp
    mov ecx , dword [ebp + 0]
    ; save eip
    mov eax , dword [ebp + 4]
    ; this_fiber in edx
    mov edx , dword [ebp + 8]
    ; save the store regs
    ;store ebx , edi , esi 
    mov dword[edx + 12] , ebx 
    mov dword[edx + 16] , edi 
    mov dword[edx + 20] , esi 
    ; this_fiber in edi 
    mov edi , edx 
    ; target fiber addr in esi
    mov edx , dword[ebp + 12]


    ; move eip to this_fiber
    ; 0 offset from fiber struct
    mov dword[edi + 0] , eax 
    ; move ebp to stack frame ptr
    ; 4 offset from fiber
    mov dword[edi + 4] , ecx  
    ; caculate the right stack pointer
    mov ecx , esp
    ; 8 bytes total are taken 
    ; from ebp and eip 
    add ecx , 8 
    ; move esp to stack pointer
    ; 8 offset from fiber str
    mov dword[edi + 8] , ecx 
    ; save store registers
    
    ; now the state of this fiber
    ; is saved is time to switch 
    ; fiber 




    ; load fiber stack frame ptr
    mov ebp , dword [edx + 4]
    ; load fiber stack pointer
    mov esp , dword [edx + 8]
    ; load the store registers
    mov ebx , dword [edx + 12]
    mov edi , dword [edx + 16]
    mov esi , dword [edx + 20]
    ; load to register instr ptr
    mov eax , dword [edx + 0]
    ; jump to this address
    jmp eax

    pop ebp 
    ret 