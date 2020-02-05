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
        uint64_t *rbp = frame->stack + STACK_SIZE - 1;
        if (!frame->value)
        {
            void *func = frame->func;
            initial_call(frame, rbp, func);
        }
        else
        {
            back_to_coroutine(*(frame->context_coroutine), rbp);
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