#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>

#include "coroutine.h"

#define STACK_SIZE 4096

void show_info(coroutine *coroutine)
{
    printf("function pointer: 0x%p\n", coroutine->func);
    printf("current result pointer: 0x%p\n", coroutine->value);
    printf("argument pointer: 0x%p\n", coroutine->env);
    printf("stack top pointer: 0x%p\n", coroutine->stack);
    printf("stack bottom pointer: 0x%p\n", coroutine->stack + STACK_SIZE - 1);
}

coroutine *new_coroutine(void (*func)(coroutine *), void *env)
{
    coroutine *result = (coroutine *)malloc(sizeof(coroutine));
    result->value = NULL;
    result->env = env;
    result->func = func;
    result->context_coroutine = (jmp_buf *)malloc(sizeof(jmp_buf));
    result->context_caller = (jmp_buf *)malloc(sizeof(jmp_buf));
    result->stack = (uint64_t *)malloc(sizeof(uint64_t) * STACK_SIZE);

    return result;
}

void yield(coroutine *frame, void *value)
{
    frame->value = value;
    if (setjmp(*(frame->context_coroutine)) == 0)
    {
        longjmp(*(frame->context_caller), 1);
    }
}

void *call_coroutine(coroutine *frame)
{
    if (setjmp(*(frame->context_caller)) == 0)
    {
        //
        if (!frame->value)
        {
            uint64_t *rsp = frame->stack + STACK_SIZE - 1;
            void *func = frame->func;
            // オペランド制約 https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
            __asm__(
                "mov rdi, %0\n\t"
                "mov rdx, rbp\n\t"
                "mov rsi, rsp\n\t"
                "mov rbp, %1\n\t"
                "mov rsp, rbp\n\t"
                "push rdx\n\t"
                "push rsi\n\t"
                "call %2\n\t" // not return if it yields.
                "pop rsi\n\t" // If return, it starts here. We should restore rsp and rbp.
                "pop rdx\n\t"
                "mov rbp, rdx\n\t"
                "mov rsp, rsi\n\t"
                :
                : "a"(frame), "b"(rsp), "c"(func)
                : "rdi", "rdx", "rsi");
        }
        else
        {
            longjmp(*(frame->context_coroutine), 1);
        }

        // if reached
        frame->value = NULL;
        return NULL;
    }
    else
    {
        return frame->value;
    }
}