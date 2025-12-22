#ifndef TYPES_H

#define CAPACITY 256

typedef enum
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
} OPERATION;

typedef struct {
    size_t len;
    double arr[CAPACITY];
} Stack;

#endif