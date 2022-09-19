#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"

#include "airboy_display.h"
#include "airboy_display_primitives.h"

#define TASKNAME "main"

void app_main(void)
{
	frame_buffer_config_t config = {
		.width = 320,
		.height = 240,
		.alloc_flags = MALLOC_CAP_DMA,
		.buffer_count = 1
	};

	init_display(&config);
	clear_buffer(0x4567);

	Line line = {
		.x0 = 20,
		.y0 = 20,
		.x1 = 50,
		.y1 = 70,
		.color = 0xC0FE
	};

	Rectangle rect = {
		.x = 0,
		.y = 0,
		.width = 20,
		.height = 20,
		.color = 0xC0FE
	};

	draw_line(line);
	draw_rect(rect);

	draw_frame();
	draw_line(line);
}
