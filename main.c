#include "coroutine.h"
#include <stdio.h>
#include <stdint.h>

void count(coroutine *frame)
{
    int hoge = 0;
    printf("address of local variable in coroutine: 0x%p\n", &hoge);
    for (int i = 0; i < *((int64_t *)frame->env); i++)
    {
        puts("coroutine called");
        yield(frame, &i);
    }
}

int main()
{
    int64_t upper = 3;
    coroutine *cor_count = new_coroutine(count, (void *)&upper);
    int *ret;

    printf("address of local variable in main function: 0x%p\n", &upper);
    show_info(cor_count);
    while ((ret = call_coroutine(cor_count)) != NULL)
    {
        printf("%d\n", *ret);
    }

    show_info(cor_count);
    return 0;
}