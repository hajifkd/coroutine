#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>

#ifndef _COROUTINE_H_
#define _COROUTINE_H_

typedef struct __coroutine
{
    void *value;
    void *env;
    void (*func)(struct __coroutine *);
    jmp_buf *context_coroutine;
    jmp_buf *context_caller;
    jmp_buf *context_back;
    uint64_t *stack;
} coroutine;

extern coroutine *new_coroutine(void (*func)(coroutine *), void *env);
extern void *call_coroutine(coroutine *frame);
extern void yield(coroutine *frame, void *value);
extern void show_info(coroutine *coroutine);
extern void initial_call(coroutine *frame, uint64_t *rbp, void *func);
extern void back_to_coroutine(jmp_buf __env, uint64_t *rbp, jmp_buf caller);

#endif