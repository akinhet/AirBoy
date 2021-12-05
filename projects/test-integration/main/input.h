#pragma once

//#include <stdint.h>

typedef struct{
	uint16_t dpad_up		: 1; //  7
	uint16_t dpad_down		: 1; //  2
	uint16_t dpad_left		: 1; //  5
	uint16_t dpad_right		: 1; //  4
	uint16_t select			: 1; //  3
	uint16_t start			: 1; //  0
	uint16_t bumper_left	: 1; //  6
	uint16_t bumper_right	: 1; // 10
	uint16_t a				: 1; //  9
	uint16_t b				: 1; //  1
	uint16_t x				: 1; // 11
	uint16_t y				: 1; //  8
} Input;

void setupInput(void);
Input pollInput(void);
