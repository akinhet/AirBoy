#include <stdlib.h>
#include "sprite.h"

typedef struct {
	Sprite *array;
	long used;
	long size;
} Array;


void initArray(Array *a, long size);
void insertArray(Array *a, Sprite element);
void freeArray(Array *a);
void removeArray(Array *a, long index);
