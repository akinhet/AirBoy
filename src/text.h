#pragma once
#include "sprite.h"

const Tileset defaultFont;


void defaultDrawText(int x, int y, const char *str, uint16_t *buffer);
void drawText(Tileset font, int x, int y, const char *str, uint16_t *buffer);
