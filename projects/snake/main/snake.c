#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_random.h>

#include "display.h"
#include "input.h"
#include "array.h"
#include "text.h"

#include "graphics.h"

#define TASKNAME "main"

static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];

const Sprite body_segment = {
	64, 64,
	16, 16,
	snake_tiles[5],
	1,0,0,0,0,0,
};

Sprite apple = {
	128, 128,
	16, 16,
	apple_sprite,
	1,0,0,0,0,0,
};

enum {
	UP = 0,
	DOWN = 1,
	LEFT,
	RIGHT
} direction = DOWN,
	prevdir = DOWN;

enum {
	PAUSE = 0,
	PLAY = 1,
	LOSE,
	MENU,
} state = MENU;

enum {
	EASY = 400000,
	MEDIUM = 200000,
	HARD = 100000,
	EXTRAHARD = 50000,
} difficulty = EASY;


int clamp(int a, int min, int max)
{
    return a > min ? (a < max ? a : max) : min;
}


void app_main(void)
{
	setupDisplay();
	setupInput();

	Input input;
	bool heldstart = false,
		heldselect = false;
	Array player;
	initArray(&player, 10);
	insertArray(&player, body_segment);
	insertArray(&player, body_segment);
	insertArray(&player, body_segment);

	long long int time = esp_timer_get_time();

	while (true) {
		input = pollInput();

		if (input.dpad_up && prevdir != DOWN)
			direction = UP;
		else if (input.dpad_right && prevdir != LEFT)
			direction = RIGHT;
		else if (input.dpad_left && prevdir != RIGHT)
			direction = LEFT;
		else if (input.dpad_down && prevdir != UP)
			direction = DOWN;

		if (input.start) {
			if (state == LOSE)
				esp_restart();
			else if (state == MENU)
				state = PLAY;
			else if (!heldstart) {
				if (state == PLAY)
					state = PAUSE;
				else if (state == PAUSE)
					state = PLAY;
			}
			heldstart = true;
		} else
			heldstart = false;

		if (input.menu && state == MENU) 
			difficulty = EXTRAHARD;

		if (input.select && state == MENU) {
			if (!heldselect) {
				if (difficulty == EASY)
					difficulty = MEDIUM;
				else if (difficulty == MEDIUM)
					difficulty = HARD;
				else if (difficulty == HARD)
					difficulty = EASY;
			}
			heldselect = true;
		} else
			heldselect = false;


		if (esp_timer_get_time() - time > difficulty && state == PLAY) {
			time = esp_timer_get_time();

			for (int i = player.used - 1; i > 0; i--) {
				player.array[i].x = player.array[i-1].x;
				player.array[i].y = player.array[i-1].y;
				player.array[i].image = player.array[i-1].image;
			}

			switch (direction) {
				case UP:
					player.array[0].y -= 16;
					break;
				case DOWN:
					player.array[0].y += 16;
					break;
				case LEFT:
					player.array[0].x -= 16;
					break;
				case RIGHT:
					player.array[0].x += 16;
					break;
			}

			prevdir = direction;
			player.array[0].x = clamp(player.array[0].x, 0, LCD_WIDTH - 16);
			player.array[0].y = clamp(player.array[0].y, 0, LCD_HEIGHT - 16);

			if (player.array[0].x == apple.x && player.array[0].y == apple.y) {
				insertArray(&player, player.array[player.used - 1]);
				apple.x = (int) (1.0 * 20 / 4294967295 * esp_random()) * 16;
				apple.y = (int) (1.0 * 15 / 4294967295 * esp_random()) * 16;
			}

			for (int i = player.used - 1; i > 0; i--) {
				if (player.array[0].x == player.array[i].x && player.array[0].y == player.array[i].y) {
					state = LOSE;
				}
			}

			if (difficulty == EXTRAHARD)
				memset(framebuffer, esp_random(), sizeof(framebuffer));
			else
				memset(framebuffer, 0, sizeof(framebuffer));
			/*esp_fill_random(framebuffer, sizeof(framebuffer));*/

			player.array[0].image = &snake_tiles[direction];

			{
				int ix = player.array[1].x,
					iy = player.array[1].y,
					px = player.array[0].x,
					py = player.array[0].y,
					nx = player.array[2].x,
					ny = player.array[2].y;

				if (py == iy && ny == iy)
					player.array[1].image = &snake_tiles[4];
				else if (px == ix && nx == ix)
					player.array[1].image = &snake_tiles[5];
				else if ((px == ix - 16 && ny == iy + 16) || (nx == ix - 16 && py == iy + 16))
					player.array[1].image = &snake_tiles[6];
				else if ((px == ix + 16 && ny == iy + 16) || (nx == ix + 16 && py == iy + 16))
					player.array[1].image = &snake_tiles[7];
				else if ((px == ix - 16 && ny == iy - 16) || (nx == ix - 16 && py == iy - 16))
					player.array[1].image = &snake_tiles[8];
				else if ((px == ix + 16 && ny == iy - 16) || (nx == ix + 16 && py == iy - 16))
					player.array[1].image = &snake_tiles[9];
			}
			{
				int ix = player.array[player.used - 1].x,
					iy = player.array[player.used - 1].y,
					px = player.array[player.used - 2].x,
					py = player.array[player.used - 2].y;

				if (py == iy - 16 && px == ix)
					player.array[player.used - 1].image = &snake_tiles[10];
				else if (py == iy + 16 && px == ix)
					player.array[player.used - 1].image = &snake_tiles[11];
				else if (px == ix + 16 && py == iy)
					player.array[player.used - 1].image = &snake_tiles[12];
				else if (px == ix - 16 && py == iy)
					player.array[player.used - 1].image = &snake_tiles[13];
			}

			drawSprite(apple, framebuffer);
			for (int i = 0; i < player.used; i++) {
				drawSprite(player.array[i], framebuffer);
			}
		}

		if (state == LOSE){
			char strbuf[128] = "";
			drawText(50, 80, 4, 0xFBDE, "Game Over", framebuffer);
			sprintf(strbuf, "Score: %ld", player.used - 3);
			drawText(50, 112, 3, 0xFBDE, strbuf, framebuffer);
		} else if (state == PAUSE) {
			drawText(50, 80, 4, 0xFBDE, "Paused", framebuffer);
			drawText(50, 112, 1, 0xFBDE, "Press start to unpause", framebuffer);
		} else if (state == MENU) {
			memset(framebuffer, 0, sizeof(framebuffer));
			drawText(50, 80, 4, 0xFBDE, "Snake", framebuffer);
			drawText(68, 128, 2, 0xFBDE, "Easy", framebuffer);
			drawText(68, 152, 2, 0xFBDE, "Medium", framebuffer);
			drawText(68, 176, 2, 0xFBDE, "Hard", framebuffer);
			if (difficulty == EASY)
				drawText(50, 128, 2, 0xFBDE, ">", framebuffer);
			else if (difficulty == MEDIUM)
				drawText(50, 152, 2, 0xFBDE, ">", framebuffer);
			else if (difficulty == HARD)
				drawText(50, 176, 2, 0xFBDE, ">", framebuffer);
		}

		frameDraw(framebuffer);
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}
