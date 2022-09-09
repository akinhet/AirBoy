#pragma once
#include <string.h>
#include <stdlib.h>
#include "display.h"

const int defaultFont[96][48];

void drawText(int x, int y, int scale, uint16_t color, const char *str, uint16_t *buffer);
