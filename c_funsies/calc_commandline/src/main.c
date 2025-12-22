#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include <stdbool.h>


extern void push(Stack *stack, double val);
extern double pop(Stack *stack);
extern double peek(Stack *stack);

double perform_op(double f, char op, double s)
{
    switch(op)
    {
        case '+':
            return f + s;
        case '-':
            return f - s;
        case '*':
            return f * s;
        case '/':
            return f / s;
        default: 
            printf("Unkown operation: %d\n", op);
            abort();
    }
}

char **evaluate_high_precedence(char **expr, int *len)
{
    int i = 0;
    while (i < *len)
    {
        if (strcmp(expr[i], "*") == 0 || strcmp(expr[i], "/") == 0)
        {
            double val1 = strtod(expr[i-1], NULL);
            char op = expr[i][0];
            double val2 = strtod(expr[i+1], NULL);
            double ans = perform_op(val1, op, val2);
            

            char *ans_str = malloc(256 * sizeof(char));
            snprintf(ans_str, 256, "%.2f", ans);
            expr[i-1] = ans_str;


            for (int j = i; j < *len - 2; j++)
            {
                expr[j] = expr[j+2];
                expr[j + 2] = "NULL";
            }

            *len -= 2;
            i = 0;
            continue;
        }
        i++;
    }

    return expr;
}

double evaluate_easy(char **easy, int len)
{
    double total = strtod(easy[0], NULL);

    for (int i = 0; i < len; i++)
    {
        if (strcmp(easy[i], "+") == 0 || strcmp(easy[i], "-") == 0)
        {
            total = perform_op(total, easy[i][0], strtod(easy[i+1], NULL));
        }
    }
    
    return total;
}

int main(int argc, char *argv[])
{   
    argc--;
    char **easy = evaluate_high_precedence(&argv[1], &argc);

    double ans = evaluate_easy(easy, argc);

    printf("%.2f\n", ans);
    return EXIT_SUCCESS;
}