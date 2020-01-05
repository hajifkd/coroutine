# Stackful coroutine in C

## Compile

```sh
gcc -masm=intel -std=c11 -Wall coroutine.c main.c
./a.out
```

## Usage

### Declare a function

```c
void fuga(coroutine *frame)
{
    // arguments can be accessed via frame->env
    int piyo = *((int *)(frame->env)) + 10;
    // call yield to yield a value.
    yield(frame, &piyo);
    // if NULL is yielded, the coroutine ends
    // yield(frame, NULL);
    // or just returning ends the coroutine as well.
}
```

### Create a context

```c
coroutine *hoge = new_coroutine(function_to_call, (void *)&arguments);
```

### Call the coroutine

```c
while ((ret = call_coroutine(context)) != NULL)
{
    printf("%d\n", *ret);
}
```

## How is it realized?

To be filled later.