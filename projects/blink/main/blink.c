#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LED 14

void app_main(void)
{
	char *taskName = pcTaskGetName(NULL);
	ESP_LOGI(taskName, "Hello, starting up\n");

	gpio_reset_pin(LED);
	gpio_set_direction(LED, GPIO_MODE_OUTPUT);

	while (1) {
		gpio_set_level(LED, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(LED, 0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
