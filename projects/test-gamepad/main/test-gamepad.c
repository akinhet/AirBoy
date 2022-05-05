#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "input.h"

#define TAG "test-gamepad"

void app_main(void)
{
	// log starting message
	ESP_LOGI(TAG, "Hello, starting up\n");

	Input input;

	setupInput();

	while (1) {
		input = pollInput();

		if (input.menu)
			ESP_LOGI(TAG, "Menu button clicked");
		if (input.dpad_up)
			ESP_LOGI(TAG, "Dpad up button clicked");
		if (input.dpad_down)
			ESP_LOGI(TAG, "Dpad down button clicked");
		if (input.dpad_left)
			ESP_LOGI(TAG, "Dpad left button clicked");
		if (input.dpad_right)
			ESP_LOGI(TAG, "Dpad right button clicked");
		if (input.select)
			ESP_LOGI(TAG, "Select button clicked");
		if (input.start)
			ESP_LOGI(TAG, "Start button clicked");
		if (input.a)
			ESP_LOGI(TAG, "A button clicked");
		if (input.b)
			ESP_LOGI(TAG, "B button clicked");
		if (input.x)
			ESP_LOGI(TAG, "X button clicked");
		if (input.y)
			ESP_LOGI(TAG, "Y button clicked");
		if (input.bumper_left)
			ESP_LOGI(TAG, "Left bumper button clicked");
		if (input.bumper_right)
			ESP_LOGI(TAG, "Right bumper button clicked");

		vTaskDelay(500/portTICK_RATE_MS);
	}
	esp_restart();
}
