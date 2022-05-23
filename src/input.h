#pragma once
#include <string.h>
#include <esp_log.h>
#include <driver/i2c.h>

#define INPUT_CLK_SPEED 1000000

#define INPUT_I2C_ADDR_0	0x27
#define INPUT_I2C_ADDR_1	0x26

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
