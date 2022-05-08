#pragma once

#define INPUT_CLK_SPEED 800000

//#define INPUT_LOGGING //uncomment to recive informational logs in console

typedef struct{
	uint16_t dpad_up		: 1;
	uint16_t dpad_down		: 1;
	uint16_t dpad_left		: 1;
	uint16_t dpad_right		: 1;
	uint16_t select			: 1;
	uint16_t menu			: 1;
	uint16_t start			: 1;
	uint16_t bumper_left	: 1;
	uint16_t bumper_right	: 1;
	uint16_t a				: 1;
	uint16_t b				: 1;
	uint16_t x				: 1;
	uint16_t y				: 1;
} Input;

void setupInput(void);
Input pollInput(void);
