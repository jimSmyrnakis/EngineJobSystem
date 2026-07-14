bits 64
section .text

global _fiber_switch_direct
_fiber_switch_direct:
    push rbp
    mov rbp , rsp

    
    ; save previus rbp
    mov r8 , qword [rbp + 0]
    ; save rip
    mov rax , qword [rbp + 8]
    ; this_fiber in rdi
    ; target fiber addr in rsi



    ; move rip to this_fiber
    ; 0 offset from fiber struct
    mov qword[rdi + 0] , rax 
    ; move rbp to stack frame ptr
    ; 8 offset from fiber
    mov qword[rdi + 8] , r8  
    ; caculate the right stack pointer
    mov rcx , rsp
    ; 16 bytes total are taken 
    ; from rbp and rip 
    add rcx , 16 
    ; move rsp to stack pointer
    ; 16 offset from fiber str
    mov qword[rdi + 16] , rcx 
    ; save store registers
    ; store rbx
    mov qword[rdi + 24] , rbx 
    ;store r12 - r15
    mov qword[rdi + 32] , r12 
    mov qword[rdi + 40] , r13 
    mov qword[rdi + 48] , r14 
    mov qword[rdi + 56] , r15 
    ; now the state of this fiber
    ; is saved is time to switch 
    ; fiber 




    ; load fiber stack frame ptr
    mov rbp , qword [rsi + 8]
    ; load fiber stack pointer
    mov rsp , qword [rsi + 16]
    ; load the store registers
    mov rbx , qword [rsi + 24]
    mov r12 , qword [rsi + 32]
    mov r13 , qword [rsi + 40]
    mov r14 , qword [rsi + 48]
    mov r15 , qword [rsi + 56]
    ; load to register instr ptr
    mov rax , qword [rsi + 0]
    ; jump to this address
    jmp rax

    pop rbp 
    ret 