#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>

#include "input.h"

enum input_pins {
	INPUT_CLK		= 25,
	INPUT_LATCH		= 33,
	INPUT_IN		= 32,
};

void setupInput()
{
	// set up pins
	gpio_reset_pin(INPUT_CLK);
	gpio_set_direction(INPUT_CLK, GPIO_MODE_OUTPUT);
	gpio_set_pull_mode(INPUT_CLK, GPIO_PULLUP_ONLY);
	
	gpio_reset_pin(INPUT_LATCH);
	gpio_set_direction(INPUT_LATCH, GPIO_MODE_OUTPUT);
	gpio_set_pull_mode(INPUT_LATCH, GPIO_PULLUP_ONLY);

	gpio_reset_pin(INPUT_IN);
	gpio_set_direction(INPUT_IN, GPIO_MODE_INPUT);
	gpio_set_pull_mode(INPUT_IN, GPIO_PULLUP_ONLY);
}

Input pollInput()
{
	// refresh bit shifter
	gpio_set_level(INPUT_LATCH, 0);
	vTaskDelay(3 / portTICK_PERIOD_MS);
	gpio_set_level(INPUT_CLK, 1);
	gpio_set_level(INPUT_LATCH, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	Input input;

	// get pin states
	input.start = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.b = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.dpad_down = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.select = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.dpad_right = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.dpad_left = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.bumper_left = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.dpad_up = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.y = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.a = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.bumper_right = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	input.x = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);

	return input;
}
