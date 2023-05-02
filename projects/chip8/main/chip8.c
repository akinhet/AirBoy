#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_random.h>
#include <esp_timer.h>
#include <esp_log.h>

#include "display.h"
/* #include "input.h" */
/* #include "array.h" */
/* #include "text.h" */

#include "stack.h"

#define TASKNAME "chip8"

#define FONT_ADDR 0x50
#define PROGRAM_ADDR 0x200

static void delay_callback(void *arg);
static void sound_callback(void *arg);
static void cpu_callback(void *arg);
static void gpu_callback(void *arg);
void draw_routine(uint16_t regx, uint16_t regy, uint16_t shortnum);

static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];
static Stack stack = {-1};

const uint16_t program[] = {
  0x00, 0xe0, 0xa2, 0x2a, 0x60, 0x0c, 0x61, 0x08, 0xd0, 0x1f, 0x70, 0x09,
  0xa2, 0x39, 0xd0, 0x1f, 0xa2, 0x48, 0x70, 0x08, 0xd0, 0x1f, 0x70, 0x04,
  0xa2, 0x57, 0xd0, 0x1f, 0x70, 0x08, 0xa2, 0x66, 0xd0, 0x1f, 0x70, 0x08,
  0xa2, 0x75, 0xd0, 0x1f, 0x12, 0x28, 0xff, 0x00, 0xff, 0x00, 0x3c, 0x00,
  0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff,
  0x00, 0x38, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x38, 0x00, 0xff, 0x00, 0xff,
  0x80, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x80, 0x00, 0x80, 0x00, 0xe0, 0x00,
  0xe0, 0x00, 0x80, 0xf8, 0x00, 0xfc, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x3b,
  0x00, 0x39, 0x00, 0xf8, 0x00, 0xf8, 0x03, 0x00, 0x07, 0x00, 0x0f, 0x00,
  0xbf, 0x00, 0xfb, 0x00, 0xf3, 0x00, 0xe3, 0x00, 0x43, 0xe5, 0x05, 0xe2,
  0x00, 0x85, 0x07, 0x81, 0x01, 0x80, 0x02, 0x80, 0x07, 0xe5, 0x05, 0xe7
};

// uint16_t for performance reasons
static uint16_t memory[4096] = {0};
static uint16_t display[64][32] = {0};
static uint16_t keys[16] = {0};
const uint16_t font[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
static int drawing = 0; // flag for screen change

static uint16_t PC = 512,
	I = 0,
	V[16] = {}; // registers

const esp_timer_create_args_t create_delay = {
	.name = "delay",
	.callback = &delay_callback,
};
static esp_timer_handle_t delay_handle;
static int delay = 0;

const esp_timer_create_args_t create_sound = {
	.name = "sound",
	.callback = &sound_callback,
};
static esp_timer_handle_t sound_handle;
static int sound = 0;

const esp_timer_create_args_t create_cpu = {
	.name = "cpu",
	.callback = &cpu_callback,
};
static esp_timer_handle_t cpu_handle;

const esp_timer_create_args_t create_gpu = {
	.name = "gpu",
	.callback = &gpu_callback,
};
static esp_timer_handle_t gpu_handle;


static void delay_callback(void *arg)
{
	if (delay > 0) {
		delay--;
		ESP_LOGI(TASKNAME, "tick");
	} else
		esp_timer_stop(delay_handle);
}


static void sound_callback(void *arg)
{
	if (sound > 0) {
		sound--;
		ESP_LOGI(TASKNAME, "beep");
	} else
		esp_timer_stop(sound_handle);
}


static void cpu_callback(void *arg)
{
	uint16_t instruction = (memory[PC] << 8) | memory[PC+1];
	PC += 2;
	uint16_t opcode = (instruction & 0xF000) >> 12,
		address = instruction & 0x0FFF,
		regx = (instruction & 0x0F00) >> 8,
		regy = (instruction & 0x00F0) >> 4,
		number = instruction & 0x00FF,
		shortnum = instruction & 0x000F;

	switch (opcode) {
		case 0x0:
			if (instruction == 0x00E0) {
				memset(display, 0, sizeof(display));
				drawing = 1;
			} else if (instruction == 0x00EE) {
				PC = pop(&stack);
			}
			break;
		case 0x1:
			PC = address;
			break;
		case 0x2:
			push(&stack, PC);
			PC = address;
			break;
		case 0x3:
			if (V[regx] == number)
				PC += 2;
			break;
		case 0x4:
			if (V[regx] != number)
				PC += 2;
			break;
		case 0x5:
			if (V[regx] == V[regy])
				PC += 2;
			break;
		case 0x6:
			V[regx] = number;
			break;
		case 0x7:
			V[regx] += number;
			break;
		case 0x8:
			switch(shortnum) {
				case 0:
					V[regx] = V[regy];
					break;
				case 1:
					V[regx] |= V[regy];
					break;
				case 2:
					V[regx] &= V[regy];
					break;
				case 3:
					V[regx] ^= V[regy];
					break;
				case 4:
					V[regx] += V[regy];
					if (V[regx] > 255) {
						V[regx] -= 255;
						V[0xF] = 1;
					} else
						V[0xF] = 0;
					break;
				case 5:
					V[regx] -= V[regy];
					if (V[regx] < 0) {
						V[regx] += 255;
						V[0xF] = 0;
					} else
						V[0xF] = 1;
					break;
				case 6:
					V[regx] = (V[regy] >> 1) & 0xFF;
					V[0xF] = V[regy] & 1;
					break;
				case 7:
					V[regx] = V[regy] - V[regx];
					if (V[regx] < 0) {
						V[regx] += 255;
						V[0xF] = 0;
					} else
						V[0xF] = 1;
					break;
				case 0xE:
					V[regx] = (V[regy] << 1) & 0xFF;
					V[0xF] = V[regy] & 0x80;
					break;
			}
			break;
		case 0x9:
			if (V[regx] != V[regy])
				PC += 2;
			break;
		case 0xA:
			I = address;
			break;
		case 0xB:
			PC = V[0] + address;
			break;
		case 0xC:
			V[regx] = 1.0 * 255 / 4294967295 * esp_random();
			V[regx] &= number;
			break;
		case 0xD:
			draw_routine(regx, regy, shortnum);
			break;
		case 0xE: // TODO: keys
			if (number == 0x9E) {
				if (keys[V[regx]])
					PC += 2;
			} else if (number == 0xA1) {
				if (!keys[V[regx]])
					PC += 2;
			}
		case 0xF:
			switch (number) {
				case 0x07:
					V[regx] = delay;
					break;
				case 0x0A: // TODO: read a key
				case 0x15:
					delay = V[regx];
					break;
				case 0x18:
					sound = V[regx];
					break;
				case 0x1E:
					I += V[regx];
					break;
				case 0x29:
					I = FONT_ADDR + V[regx] * 5;
					break;
				case 0x33:
					{
						int input = V[regx];
						for (int i = 2; i > 0; i--) {
							memory[I + i] = input % 10;
							input /= 10;
						}
					}
					break;
				case 0x55:
					for (int i = 0; i <= regx; i++) {
						memory[I] = V[i];
						I++;
					}
					break;
				case 0x65: // TODO: decide on ambiguity
					for (int i = 0; i <= regx; i++) {
						V[i] = memory[I];
						I++;
					}
					break;
			}
			break;
	}
}


static void gpu_callback(void *arg)
{
	if (drawing) {
		memset(framebuffer, 0, sizeof(framebuffer));
		for (int y = 0; y < 31; y++)
			for (int x = 0; x < 63; x++) {
				if (display[x][y]) {
					for (int i = 0; i < 5; i++)
						for (int j = 0; j < 5; j++)
							framebuffer[LCD_WIDTH * (y*5 + i) + x*5 + j] = 0xFFFF;
				}
			}

		drawing = 0;
		frameDraw(framebuffer);
	}
}


void draw_routine(uint16_t regx, uint16_t regy, uint16_t shortnum)
{
	int x = V[regx] & 63,
		y = V[regy] & 31,
		width = x + 8 > 63 ? 63 - x : 8,
		height = y + shortnum > 31 ? 31 - y : shortnum;

	V[0xF] = 0;

	/* for (int i = 0; i < shortnum; i++) */
	/* 	for (int j = 0; j < width; j++) { */
	/* 		int pixel = display[x + j][y + i] ^ (memory[I + i] & (0x80 >> j)); */
	/* 		if (!pixel) */
	/* 			V[0xF] = 1; */
	/* 		display[x + (7 - j)][y + i] = pixel; */
	/* 	} */
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (memory[I + i] & (0x80 >> j)) {
				if (display[x + j][y + i])
					V[0xF] = 1;
				display[x + j][y + i] ^= 1;
			}
		}

	drawing = 1;
}


void app_main(void)
{
	// Initialize timers
	ESP_ERROR_CHECK(esp_timer_create(&create_delay, &delay_handle));
	ESP_ERROR_CHECK(esp_timer_create(&create_sound, &sound_handle));
	ESP_ERROR_CHECK(esp_timer_create(&create_cpu, &cpu_handle));
	ESP_ERROR_CHECK(esp_timer_create(&create_gpu, &gpu_handle));
	ESP_ERROR_CHECK(esp_timer_start_periodic(delay_handle, 16.666 * 1000));
	ESP_ERROR_CHECK(esp_timer_start_periodic(sound_handle, 16.666 * 1000));

	// Initialize memory
	memset(memory, 0, sizeof(memory));
	memset(display, 0, sizeof(display));
	memcpy(memory + FONT_ADDR, font, 160);
	memcpy(memory + PROGRAM_ADDR, program, sizeof(program));

	setupDisplay();

	ESP_ERROR_CHECK(esp_timer_start_periodic(gpu_handle, 16.666 * 1000));
	ESP_ERROR_CHECK(esp_timer_start_periodic(cpu_handle, 14 * 1000));

	vTaskDelay(1000 / portTICK_PERIOD_MS);
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++)
			printf("%d", display[x][y]);
		printf("\n");
	}

	while (true) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
