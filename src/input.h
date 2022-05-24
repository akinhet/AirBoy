#pragma once

#define INPUT_CLK_SPEED 800000

#define INPUT_I2C_ADDR_0	0x27
#define INPUT_I2C_ADDR_1	0x26

#define INPUT_I2C_PORT		0

#define INPUT_REG_IODIR		0x00
#define INPUT_REG_IPOL		0x01
#define INPUT_REG_IOCON		0x05
#define INPUT_REG_GPPU		0x06
#define INPUT_REG_GPIO		0x09

#define TIME_OUT 1000
#define TIME_DELAY 1000

#define ACK_CHECK_EN 0x1 
#define ACK_VAL    0x0
#define NACK_VAL   0x1

#define TAG "Input"

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
esp_err_t mcp23008_write_reg(uint8_t ICAddress, uint8_t regAddress, uint8_t value);
esp_err_t mcp23008_read_reg(uint8_t ICAddress, uint8_t regAddress, uint8_t* value);
