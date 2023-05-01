#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 256

typedef struct {
	long top;
	uint16_t data[MAXSIZE];
} Stack;

uint16_t isempty(Stack *stack);
uint16_t isfull(Stack *stack);
uint16_t pop(Stack *stack);
void push(Stack *stack, uint16_t value);
