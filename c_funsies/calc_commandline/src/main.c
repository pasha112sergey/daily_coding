#include <stdio.h>
#include <stdlib.h>
#include "../include/types.h"
#include <string.h>
#include "stack.c"

OPERATION match_operation(char *op)
{
        if(strcmp(op, "+") == 0)
            return ADD;
        if(strcmp(op, "-") == 0)
            return SUBTRACT;
        if(strcmp(op, "*") == 0)
            return MULTIPLY;
        if(strcmp(op, "/") == 0)
            return DIVIDE;
        return -1;
    
}

double perform_op(double f, OPERATION op, double s)
{
    switch(op)
    {
        case ADD:
            return f + s;
        case SUBTRACT:
            return f - s;
        case MULTIPLY:
            return f * s;
        case DIVIDE:
            return f / s;
        default: 
            printf("Unkown operation: %d\n", op);
            abort();
    }
}

int main(int argc, char *argv[])
{   
    Stack *stack = calloc(1, sizeof(Stack));

    for (int i = 1; i < argc; i++)
    {
        OPERATION op;

        if ((op = match_operation(argv[i])) == -1)
        {
            double val = strtod(argv[i], NULL);
            push(stack, val);
        }

        else
        {
            double second = pop(stack);
            double first = pop(stack);
            double result = perform_op(first, op, second);

            push(stack, result);
        }
    }

    double ans = peek(stack);
    free(stack);
    printf("%.2f\n", ans);

    return EXIT_SUCCESS;
}