#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "airboy_display.h"
#include "airboy_viewport.h"
#include "airboy_shapes.h"

void app_main(void)
{
    uint8_t map[24][24] = {
        {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
        {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
        {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
        {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
        {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
        {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
        {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
        {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,1},
        {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,7,7,1},
        {4,0,7,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,1},
        {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
        {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
        {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
        {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
        {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
        {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
        {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
        {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
        {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
        {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
        {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
        {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
        {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
        {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}, 
    };

    frame_buffer_config_t fb_config = {
		.width = 320,
		.height = 240,
		.alloc_flags = MALLOC_CAP_DMA,
		.buffer_count = 1
	};

	init_display(&fb_config);

    viewport_t viewport;
    viewport_config_t vp_config = {
        .width = 320,
        .height = 240,
        .x_start = 0,
        .y_start = 0
    };

    init_viewport(&vp_config, &viewport);

    Rectangle rect = {
        .x = 400,
        .y = 120,
        .width = 30,
        .height = 30,
        .color = SWAP_BYTES(0xC0FE)
    };

    Rectangle rect2 = {
        .x = 80,
        .y = 210,
        .width = 30,
        .height = 30,
        .color = SWAP_BYTES(0xC0FE)
    };

    Rectangle tile = {.x = 0, .y = 0, .width = 32, .height = 32, .color = SWAP_BYTES(0xAB54)};

    bool side = true;
    while(true)
    {
        clear_buffer(0);

        draw_rect(rect2, NULL);

        if (side) viewport.x+=2;
        else viewport.x-=2;

        if (viewport.x > 250) side = ! side;
        if (viewport.x < 0) side = ! side;



        for (int x = (viewport.x >> 5); x < (((viewport.x + viewport.w) >> 5) + 1); x++)
            for(int y = (viewport.y >> 5); y < (((viewport.y + viewport.h) >> 5) + 1); y++)
            {
                if (map[x][y] == 0) continue;
                tile.x = (x << 5); tile.y = (y << 5);
                draw_rect(tile, &viewport);
            }

        //draw_rect(rect, &viewport);

        draw_frame();
        vTaskDelay(1);
    }

}