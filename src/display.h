#pragma once

//#include <stdint.h>

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_DEPTH 2

//static uint16_t frameBuffer[LCD_WIDTH * LCD_HEIGHT]; //Example of frame buffer declaration

void setupDisplay(void);
void frameDraw(uint16_t* buffer);
