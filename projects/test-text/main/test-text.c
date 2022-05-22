#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sprite.h"
#include "display.h"
#include "text.h"

void app_main(void)
{
	static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];
    setupDisplay();
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
		framebuffer[i] = 0x3F3F;
	drawText(20, 20, 4, 0, "Hello World!", framebuffer);
	drawText(20, 60, 3, 0, "Hello World!", framebuffer);
	drawText(20, 90, 2, 0, "Hello World!", framebuffer);
	drawText(20, 110, 1, 0, "Hello World!", framebuffer);
	while (1) {
		frameDraw(framebuffer);
		vTaskDelay(20 / portTICK_PERIOD_MS);
	}
}
