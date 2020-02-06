.intel_syntax noprefix
.globl initial_call
.globl back_to_coroutine

back_to_coroutine:
    push rbp
    mov rbp, rsp
    push rdx
    lea rax, qword ptr [rip + .back_to_caller]
    push rax
    push rbp
    lea rax, qword ptr [rsp]
    mov qword ptr [rsi - 0x8], rax /* rewrite ret addr */
    mov rsi, 1
    call longjmp

.back_to_caller:
    mov rdi, qword ptr [rbp - 0x8]
    mov rsi, 1
    call longjmp
    /* never return */

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
