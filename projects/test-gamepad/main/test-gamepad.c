#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define CLK 25
#define LATCH 33
#define INPUT 32
#define LED 14

bool states[12];
//uint16_t buttonmask = 0;
bool led;

enum{
	BUTTON_DPAD_UP		= 7,
	BUTTON_DPAD_DOWN	= 2,
	BUTTON_DPAD_LEFT	= 5,
	BUTTON_DPAD_RIGHT	= 4,
	BUTTON_SELECT		= 3,
	BUTTON_START		= 0,
	BUTTON_BUMPER_LEFT	= 6,
	BUTTON_BUMPER_RIGHT = 10,
	BUTTON_A			= 9,
	BUTTON_B			= 1,
	BUTTON_X			= 11,
	BUTTON_Y			= 8,
} keys;

void app_main(void)
{
	// log starting message
	char *taskName = pcTaskGetName(NULL);
	ESP_LOGI(taskName, "Hello, starting up\n");

	// set up pins
	gpio_reset_pin(CLK);
	gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
	gpio_set_pull_mode(INPUT, GPIO_PULLUP_ONLY);
	
	gpio_reset_pin(LATCH);
	gpio_set_direction(LATCH, GPIO_MODE_OUTPUT);
	gpio_set_pull_mode(INPUT, GPIO_PULLUP_ONLY);

	gpio_reset_pin(INPUT);
	gpio_set_direction(INPUT, GPIO_MODE_INPUT);
	gpio_set_pull_mode(INPUT, GPIO_PULLUP_ONLY);

	gpio_reset_pin(LED);
	gpio_set_direction(LED, GPIO_MODE_OUTPUT);

	gpio_set_level(LATCH, 1);
	gpio_set_level(CLK, 0);

	// main program loop
	while (1) {
		// refresh bit shfiter
		gpio_set_level(LATCH, 0);
		vTaskDelay(10 / portTICK_PERIOD_MS);
		gpio_set_level(LATCH, 1);

		// get pin states
		for (int i = 0; i < 12; i++) {
			states[i] = gpio_get_level(INPUT);
			//buttonmask |= gpio_get_level(INPUT) << i;
			gpio_set_level(CLK, 0);
			vTaskDelay(10 / portTICK_PERIOD_MS);
			gpio_set_level(CLK, 1);
		}

		for (int i = 0; i < 12; i++) {
			if (states[i] == 0)
				printf("%d\n", i);
			/*
			if (!(buttonmask & (1 << i)))
				printf("%d ", i);
			*/
		}

		// blink led if up is held down
		led = states[BUTTON_DPAD_UP] ? 0 : !led;
		/*led = ((buttonmask & 1 << BUTTON_DPAD_UP) >> BUTTON_DPAD_UP) ? 0 : !led;*/

		gpio_set_level(LED, led);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

	esp_restart();
}
