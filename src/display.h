#pragma once

#include <stdint.h>

//#define DISPLAY_LOGGING //uncomment to recive informational logs in console

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_DEPTH 2


typedef struct {
    int x,
        y,
        width,
        height;
    uint16_t color;
} Rectangle;

typedef struct {
    int x0,
        y0,
        x1,
        y1;
    uint16_t color;
} Line;


//static uint16_t frameBuffer[LCD_WIDTH * LCD_HEIGHT]; //Example of frame buffer declaration

void setupDisplay(void);
void frameDraw(uint16_t* buffer);

void drawRect(Rectangle r, uint16_t* buffer);
void drawLine(Line l, uint16_t* buffer);
