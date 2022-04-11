#include "array.h"

void initArray(Array *a, long size)
{
	a->array = (Sprite *) malloc(size * sizeof(Sprite));
	a->used = 0;
	a->size = size;
}


void insertArray(Array *a, Sprite element)
{
	if (a->used == a->size) {
		a->size += 5;
		a->array = (Sprite *) realloc(a->array, a->size * sizeof(Sprite));
	}
	a->array[a->used++] = element;
}


void freeArray(Array *a)
{
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}


void removeArray(Array *a, long index)
{
	if (index < 0)
		return;
	for (int i = index; i < a->used - 1; i++) {
		a->array[i] = a->array[i+1];
	}

	a->used--;
}
