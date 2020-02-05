.intel_syntax noprefix
.globl initial_call
.globl back_to_coroutine

back_to_coroutine:
    push rbp
    mov rbp, rsp
    lea rax, qword ptr [rbp + 0x8]
    mov qword ptr [rsi], rax
    mov rsi, 1
    call longjmp
    /*never return*/

initial_call:
    push rbp
    mov rbp, rsp
    mov rax, rbp
    mov rbp, rsi
    mov rsp, rbp

    push rax
    
    call rdx /* not return if it yields. */

    pop rax /* If return, it starts here. We should restore rsp and rbp. */
    /* Now, the stack frame is at back_coroutine */
    mov rbp, rax
    mov rsp, rbp
    leave
    ret
