#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include <stddef.h>


void print_stack(const Stack *stack)
{
    printf("length=%ld, top ele=%f\n", stack->len, stack->arr[stack->len]);

    for (int i = 0; i < stack->len; i++)
    {
        printf("%d) %.2f\n", i, stack->arr[i]);
    }
}

void push(Stack *stack, const double val)
{
    if (stack->len < CAPACITY)
    {
        stack->arr[stack->len++] = val;
    }
    else
    {
        printf("Stack overflow error\n");
        abort();
    }
}

double pop(Stack *stack)
{
    if (stack->len > 0)
    {
        return stack->arr[--stack->len];
    }
    else 
    {
        printf("Popping from empty stack error\n");
        abort();
    }
    return -1;
}

double peek(Stack *stack)
{
    return stack->arr[stack->len -1];
}
