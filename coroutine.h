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
    uint64_t *stack;
} coroutine;

extern coroutine *new_coroutine(void (*func)(coroutine *), void *env);
extern void *call_coroutine(coroutine *frame);
extern void yield(coroutine *frame, void *value);
extern void show_info(coroutine *coroutine);
extern void initial_call(coroutine *frame, uint64_t *rbp, void *func);
extern void back_to_coroutine(struct __jmp_buf_tag *__env, uint64_t *rbp);

#endif