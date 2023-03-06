#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_random.h>
#include <esp_log.h>

#include <airboy_display.h>
#include <airboy_display_framebuffer.h>
#include <airboy_shapes.h>
#include <airboy_math.h>
/* #include "display.h"
#include "input.h"
#include "array.h"
#include "text.h" */

#define TAG "fire-demo"

#define LCD_HEIGHT 240
#define LCD_WIDTH  320

/* static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT]; */
static uint8_t firebuffer[LCD_WIDTH * LCD_HEIGHT];

frame_buffer_config_t fb_config = {
    .width = 320,
	.height = 240,
	.alloc_flags = MALLOC_CAP_DMA,
	.buffer_count = 1
};

const uint16_t firepixels[72] = {
	0x0000, 0x0000, 0x2018, 0x2018, 0x6020, 0x6020, 0x6038, 0x6038,
	0x8040, 0x8040, 0xC050, 0xC050, 0xC058, 0xC058, 0xE070, 0xE070,
	0x2079, 0x2079, 0x8089, 0x8089, 0xC099, 0xC099, 0xC099, 0xC099,
	0xE0B1, 0xE0B1, 0x20B2, 0x20B2, 0x20B2, 0x20B2, 0x60AA, 0x60AA,
	0x81AA, 0x81AA, 0xC1A2, 0xC1A2, 0xE1A2, 0xE1A2, 0x21A3, 0x21A3,
	0x62A3, 0x62A3, 0x629B, 0x629B, 0xA3C4, 0xA3C4, 0xE3BC, 0xE3BC,
	0xE3BC, 0xE3BC, 0x24BD, 0x24BD, 0x24BD, 0x24BD, 0x65BD, 0x65BD,
	0x65B5, 0x65B5, 0xA5B5, 0xA5B5, 0xA6B5, 0xA6B5, 0x6DCE, 0x6DCE,
	0xF3DE, 0xF3DE, 0x78EF, 0x78EF, 0x92EF, 0x92EF, 0xFFFF, 0xFFFF
};

const uint8_t rand_array[300] = {
	89,212,161,75,249,252,102,186,147,107,
	93,174,85,155,92,148,15,123,13,57,
	163,79,180,189,217,70,18,102,222,148,
	79,159,139,12,76,154,76,1,186,245,
	130,81,166,222,15,65,79,229,86,20,
	108,75,84,46,161,139,42,253,109,192,
	163,247,138,33,20,235,46,119,161,93,
	247,53,122,126,114,229,72,204,93,201,
	195,224,179,69,22,252,240,171,54,39,
	27,139,124,197,156,9,117,243,18,232,
	249,168,189,124,232,43,189,243,193,231,
	129,139,165,253,113,217,158,164,114,197,
	186,134,96,130,82,95,134,170,37,33,
	69,80,80,116,30,86,77,15,125,56,
	118,201,181,218,146,86,226,140,170,31,
	33,51,57,119,103,224,95,43,186,164,
	98,87,252,8,207,140,14,248,223,107,
	238,55,14,178,93,64,252,126,198,131,
	83,201,184,21,22,184,218,81,19,95,
	80,101,119,208,65,63,136,40,24,208,
	209,163,218,158,1,57,123,3,11,91,
	11,253,128,123,164,237,211,8,244,91,
	61,66,202,67,54,201,5,55,193,41,
	121,250,18,49,122,19,104,219,38,161,
	1,73,68,201,214,133,131,15,122,59,
	100,196,111,191,201,10,191,123,121,157,
	179,63,66,19,112,149,156,199,27,205,
	88,102,112,242,76,220,59,123,68,164,
	132,155,207,181,12,117,45,225,68,21,
	102,216,171,87,52,171,120,127,72,234,
};
static uint16_t rand_index = 0;
static uint16_t prev_rand = 89;

int clamp(int a, int min, int max)
{
	return a > min ? (a < max ? a : max) : min;
}


uint16_t random_generator(uint16_t prev_num)
{
	int x0 = 1,
		m = 2038074743,
		a = 15485863,
		c = 2135;

	return ((prev_num * a) + c) % m;
}


void spreadfire(int from)
{
	/* int rand = (int) (rand_array[from] / UINT32_MAX * 3); */
	prev_rand = random_generator(prev_rand);
	/* ESP_LOGI(TAG, "%d", rand); */
	int to = from - LCD_WIDTH - prev_rand % 3 + 1;
	firebuffer[to] = max(firebuffer[from] - prev_rand % 3, 0);
}


void app_main(void)
{
	/* setupDisplay(); */
	/* setupInput(); */
	init_display(&fb_config);

	/* rand_array = malloc(sizeof(uint16_t)*LCD_HEIGHT*LCD_WIDTH); */
	/* rand_array = heap_caps_malloc(sizeof(uint16_t)*LCD_HEIGHT*LCD_WIDTH, MALLOC_CAP_DEFAULT); */
	/* if (rand_array == NULL) */
	/* 	ESP_LOGI(TAG, "dupa"); */

	/* Input input; */
	int frame = 0;
	memset(firebuffer, 0, sizeof(uint8_t) * LCD_WIDTH * LCD_HEIGHT);
	memset(firebuffer + LCD_WIDTH * (LCD_HEIGHT - 1), 71, sizeof(uint8_t) * LCD_WIDTH);

	long long int time = esp_timer_get_time();

	while (true) {
		time = esp_timer_get_time();
		/* input = pollInput(); */
		/* esp_fill_random(&rand_array, LCD_WIDTH * LCD_HEIGHT); */
		/* random_generator(rand_array, LCD_WIDTH * LCD_HEIGHT); */

		/* for (int x = 0; x < LCD_WIDTH; x++) */
		for (int y = LCD_HEIGHT - 133; y < LCD_HEIGHT; y++) {
			rand_index = rand_index < 300 ? rand_index + 1 : 0;
			prev_rand = rand_array[rand_index];
			for (int x = 0; x < LCD_WIDTH; x++)
				spreadfire(y * LCD_WIDTH + x);
		}

		/* memset(framebuffer, 0, sizeof(framebuffer)); */
		//memset(framebuffer, esp_random(), sizeof(framebuffer));
		//esp_fill_random(framebuffer, sizeof(framebuffer));
		clear_buffer(0);

		for (int x = 0; x < LCD_WIDTH; x++)
			for (int y = LCD_HEIGHT - 133; y < LCD_HEIGHT; y++) {
				/* spreadfire(y * LCD_WIDTH + x); */
				set_pixel(x, y, firepixels[firebuffer[y * LCD_WIDTH + x]]);
				/* framebuffer[y * LCD_WIDTH + x] = firepixels[firebuffer[y * LCD_WIDTH + x]]; */
			}

		draw_frame();
		/* frameDraw(framebuffer); */
		/* vTaskDelay(10 / portTICK_PERIOD_MS); */
		frame = frame >= 119 ? 0 : frame + 1;
		/* long long int deltatime = 33333 - esp_timer_get_time() + time; // 30 FPS */
		long long int deltatime = 16666 - esp_timer_get_time() + time; // 30 FPS
		ESP_LOGI(TAG, "%lld", deltatime);
		vTaskDelay(deltatime > 0 ? deltatime / 1000 : 0 / portTICK_PERIOD_MS);
	}
}
