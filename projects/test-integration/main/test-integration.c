#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "display.h"
#include "input.h"
#include "macros.h"

#define TASKNAME "main"

static uint16_t frameBuffer[LCD_WIDTH * LCD_HEIGHT];

void app_main(void)
{
	// log starting message
	ESP_LOGI(TASKNAME, "Hello, starting up");
	setupDisplay();
	setupInput();

	Line test = {100, 20, 15, 12, 0xFFFF};

	Rectangle rect = {100, 100, 20, 20, 0xFFFF};

	Input input;

	uint16_t color = 0xFFFF;
	uint16_t background = 0;

	int x = 0;
	int y = 0;

	int width  = 20;
	int height = 20;

	int velx = 0;
	int vely = 0;

	int gravity = 2;

	while (true) {
		input = pollInput();

		if (input.dpad_up && y+height+1 == LCD_HEIGHT)
			vely -= 22;
		//if (input.dpad_down)
		//	vely += 10;
		if (input.dpad_right)
			x += 10;
		if (input.dpad_left)
			x -= 10;
		if (input.a)
			color = SWAP_ENDIAN_16(RGB565(0xFF,0,0));
		if (input.b)
			color = SWAP_ENDIAN_16(RGB565(0,0xFF,0));
		if (input.x)
			color = SWAP_ENDIAN_16(RGB565(0,0,0xFF));
		if (input.y)
			color = SWAP_ENDIAN_16(RGB565(0xFF,0xFF,0xFF));
		if (input.start)
			background = SWAP_ENDIAN_16(RGB565(0,0xFF,0));
		if (input.select)
			background = SWAP_ENDIAN_16(RGB565(0,0,0xFF));
		if (input.bumper_left)
			background = SWAP_ENDIAN_16(RGB565(0xFF,0,0));
		if (input.bumper_right)
			background = SWAP_ENDIAN_16(RGB565(0,0,0));

		//memset(frameBuffer, 0, LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH);

		if (y+height+1 != LCD_HEIGHT)
			vely += gravity;

		y += vely;

		if (x < 0)
			x = 0;
		else if (x + width >= LCD_WIDTH)
			x = LCD_WIDTH - width - 1;
		
		if (y < 0)
			y = 0;
		else if (y + height >= LCD_HEIGHT)
			y = LCD_HEIGHT - height - 1;

		for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
			frameBuffer[i] = background;

		for (int row = y; row < y + height; row++)
			for (int col = x; col < x + width; col++)
				frameBuffer[LCD_WIDTH * row + col] = color;

		drawLine(test, frameBuffer);
		drawRect(rect, frameBuffer);

		frameDraw(frameBuffer);
	}

	esp_restart();
}
