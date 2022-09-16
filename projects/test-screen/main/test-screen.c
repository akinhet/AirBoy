#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"

#include "airboy_display.h"

#define TASKNAME "main"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_DEPTH 2

void app_main(void)
{
	init_display(LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH, MALLOC_CAP_DMA, 1);
}
