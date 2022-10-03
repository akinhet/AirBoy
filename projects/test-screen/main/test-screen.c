#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"

#include "airboy_display.h"
#include "airboy_shapes.h"

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
	clear_buffer(0xFF);

	Rectangle rect = {
		.x = 0,
		.y = 0,
		.w = 16,
		.h = 16,
		.color = 0x0360
	};

	draw_rect(&rect, NULL);

	while (true)
	{
		wait_for_end();
		clear_buffer(0x0);
		for (int x = 0; x < 320; x+=16)
			for (int y = 0; y < 240; y+=16)
			{
				rect.x = x; rect.y = y;
				draw_rect(&rect, NULL);
				
			}
		draw_frame();
		
		//ESP_LOGE(TASKNAME, "dupa2");
	}
}
