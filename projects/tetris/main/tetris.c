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

#define BLOCKSIZE 16

typedef enum {
I = 0, J, L, O, S, T, Z
} piece;

static enum {
PLAY,
PAUSE,
DEAD,
} state = PLAY;

bool bag[7];
uint8_t bag_size = 7;

static Sprite *board[15][10];

static struct {
	int x, y;
	int bx, by, bw, bh; // bounding box of the tetromino
	piece type;
	int rotation;
	Sprite *blocks[4];
} tetromino;

const uint8_t block_lookup[7][4][4][2] = {
	{ // I
		{{0,1},
		 {1,1},
		 {2,1},
		 {3,1}},
		{{2,0},
		 {2,1},
		 {2,2},
		 {2,3}},
		{{0,2},
		 {1,2},
		 {2,2},
		 {3,2}},
		{{1,0},
		 {1,1},
		 {1,2},
		 {1,3}},
	},
	{ // J
		{{0,0},
		 {0,1},
		 {1,1},
		 {2,1}},
		{{1,0},
		 {2,0},
		 {1,1},
		 {1,2}},
		{{0,1},
		 {1,1},
		 {2,1},
		 {2,2}},
		{{1,0},
		 {1,1},
		 {1,2},
		 {0,2}},
	},
	{ // L
		{{2,0},
		 {2,1},
		 {1,1},
		 {0,1}},
		{{1,0},
		 {1,1},
		 {1,2},
		 {2,2}},
		{{0,1},
		 {1,1},
		 {2,1},
		 {0,2}},
		{{0,0},
		 {1,0},
		 {1,1},
		 {1,2}},
	},
	{ // O
		{{0,0},
		 {1,0},
		 {0,1},
		 {1,1}},
		{{0,0},
		 {1,0},
		 {0,1},
		 {1,1}},
		{{0,0},
		 {1,0},
		 {0,1},
		 {1,1}},
		{{0,0},
		 {1,0},
		 {0,1},
		 {1,1}},
	},
	{ // S
		{{1,0},
		 {2,0},
		 {0,1},
		 {1,1}},
		{{1,0},
		 {1,1},
		 {2,1},
		 {2,2}},
		{{1,1},
		 {2,1},
		 {0,2},
		 {1,2}},
		{{0,0},
		 {0,1},
		 {1,1},
		 {1,2}},
	},
	{ // T
		{{1,0},
		 {0,1},
		 {1,1},
		 {2,1}},
		{{1,0},
		 {1,1},
		 {2,1},
		 {1,2}},
		{{0,1},
		 {1,1},
		 {2,1},
		 {1,2}},
		{{1,0},
		 {0,1},
		 {1,1},
		 {1,2}},
	},
	{ // Z
		{{0,0},
		 {1,0},
		 {1,1},
		 {2,1}},
		{{2,0},
		 {1,1},
		 {2,1},
		 {1,2}},
		{{0,1},
		 {1,1},
		 {1,2},
		 {2,2}},
		{{1,0},
		 {0,1},
		 {1,1},
		 {0,2}},
	},
};


int clamp(int a, int min, int max)
{
	return a > min ? (a < max ? a : max) : min;
}


void move_blocks()
{
	tetromino.bx = tetromino.by = 9999;
	tetromino.bw = tetromino.bh = 0;

	for (int i = 0; i < 4; i++) {
		tetromino.blocks[i]->x = tetromino.x + block_lookup[tetromino.type][tetromino.rotation][i][0] * BLOCKSIZE;
		tetromino.bx = tetromino.blocks[i]->x < tetromino.bx ? tetromino.blocks[i]->x : tetromino.bx;
		tetromino.bw = tetromino.blocks[i]->x + BLOCKSIZE > tetromino.bw ? tetromino.blocks[i]->x + BLOCKSIZE : tetromino.bw;
		tetromino.blocks[i]->y = tetromino.y + block_lookup[tetromino.type][tetromino.rotation][i][1] * BLOCKSIZE;
		tetromino.by = tetromino.blocks[i]->y < tetromino.by ? tetromino.blocks[i]->y : tetromino.by;
		tetromino.bh = tetromino.blocks[i]->y + BLOCKSIZE > tetromino.bh ? tetromino.blocks[i]->y + BLOCKSIZE : tetromino.bh;
	}
}
                                                                                                                                                                                                                                                                                                                            // GO FUCK YOURSELF

void set_blocks()
{
	tetromino.y = 0;
	tetromino.x = 2 * BLOCKSIZE;
	tetromino.rotation = 0;

	for (int i = 0; i < 4; i++) {
		/* memset(&tetromino.blocks[i], 0, sizeof(Sprite)); */
		tetromino.blocks[i] = malloc(sizeof(Sprite));
		memset(tetromino.blocks[i], 0, sizeof(Sprite));

		tetromino.blocks[i]->width = BLOCKSIZE;
		tetromino.blocks[i]->height = BLOCKSIZE;

		tetromino.blocks[i]->image = blocks[tetromino.type];
	}

	move_blocks();

	for (int i = 0; i < 4; i++)
		if (board[tetromino.blocks[i]->y / 16][tetromino.blocks[i]->x / 16] != NULL) {
			state = DEAD;
			break;
		}
}


void fill_bag()
{
	for (int i = 0; i < 7; i++) {
		bag[i] = 0;
	}
	bag_size = 7;
}


void generate_piece() // TODO: zasrane gówno nie działa
{
	tetromino.type = 3;
	set_blocks();
	return;

	int dice_roll = (int) (1.0 * bag_size / 4294967295 * esp_random());
	bag_size--;

	while (bag[dice_roll]) {
		if (dice_roll < 0)
			dice_roll = 6;
		else
			dice_roll--;
	}
	bag[dice_roll] = true;
	tetromino.type = dice_roll;
	set_blocks();

	if (bag_size == 0)
		fill_bag();
}


void rotate_piece(bool clockwise)
{
	int prev_rot = tetromino.rotation;
	if (clockwise)
		tetromino.rotation = tetromino.rotation >= 3 ? 0 : tetromino.rotation + 1;
	else
		tetromino.rotation = tetromino.rotation <= -1 ? 3 : tetromino.rotation - 1;

	move_blocks();
	if (tetromino.bx < 0 || tetromino.by < 0 || tetromino.bw > 10 * BLOCKSIZE || tetromino.bh > LCD_HEIGHT)
		tetromino.rotation = prev_rot;
}


void check_lines()
{
	for (int i = 1; i < 15; i++) {
		bool line = true;
		for (int j = 0; j < 10; j++) {
			if (board[i][j] == NULL) {
				line = false;
				break;
			}
		}
		if (line) {
			/* for (int j = i; j > 0; j--) { */
			/* 	memcpy(&board[j], &board[j-1], sizeof(Sprite) * 10); */
			/* } */
			for (int j = 0; j < 10; j++) {
				free(board[i][j]);
				board[i][j] = NULL;
			}
			for (int j = i; j > 0; j--)
				for (int k = 0; k < 10; k++)
					board[j][k] = board[j-1][k];
				/* memcpy(board[j][0], board[j+1][0], sizeof(Sprite) * 10); // TODO: cascading line deleting */
			for (int j = 0; j < 10; j++) {
				free(board[0][j]);
				board[0][j] = NULL;
			}
		}
	}
}


bool is_colliding()
{
	move_blocks();
	for (int i = 0; i < 4; i++) {
		if (board[tetromino.blocks[i]->y / 16][tetromino.blocks[i]->x / 16] != NULL) {
			return true;
		}
	}
	return false;
}


void check_if_sitting()
{
	bool sit = false;
	for (int i = 0; i < 4; i++) {
		if (board[tetromino.blocks[i]->y / 16 + 1][tetromino.blocks[i]->x / 16] != NULL || tetromino.bh == LCD_HEIGHT) {
			sit = true;
			break;
		}
	}

	if (sit) {
		for (int i = 0; i < 4; i++) {
			board[tetromino.blocks[i]->y / 16][tetromino.blocks[i]->x / 16] = tetromino.blocks[i];
			/* memcpy(&board[tetromino.blocks[i].x / 16][tetromino.blocks[i].y / 16 + 1], &tetromino.blocks[i], sizeof(Sprite)); */
		}
		generate_piece();
		check_lines();
	}
}


void app_main(void)
{
	setupDisplay();
	setupInput();
	fill_bag();

	Input input, prev_input;
	memset(&prev_input, 0, sizeof(Input));

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 10; j++)
			board[i][j] = NULL;

	generate_piece();

	long long int time;
	int frame = 0;
	bool change = true;
	piece hold = -1;
	uint8_t left_to = 0,
		left_counter = 0,
		right_to = 0,
		right_counter = 0;

	while (true) {
		time = esp_timer_get_time();
		prev_input = input;
		input = pollInput();

		if (input.dpad_up && !prev_input.dpad_up && state == PLAY) {
			/* tetromino.rotation = tetromino.rotation == 3 ? 0 : tetromino.rotation + 1; */
			rotate_piece(true);
			change = true;
		}

		if (input.dpad_left && state == PLAY) {
			if (left_counter == 0 || left_to == 0) {
				tetromino.x -= BLOCKSIZE;

				if (tetromino.bx <= 0 || is_colliding())
					tetromino.x += BLOCKSIZE;

				change = true;
				/* tetromino.x = tetromino.bx <= 0 ? tetromino.x : tetromino.x - BLOCKSIZE; */
				/* change = true; */
				left_counter = left_counter == 0 ? 6 : 2;
				left_to = left_counter;
			} else {
				left_to--;
			}
		} else
			left_counter = 0;

		if (input.dpad_right && state == PLAY) {
			if (right_counter == 0 || right_to == 0) {
				tetromino.x += BLOCKSIZE;

				if (tetromino.bw >= 10 * BLOCKSIZE || is_colliding())
					tetromino.x -= BLOCKSIZE;

				change = true;
				/* tetromino.x = tetromino.bw >= 10 * BLOCKSIZE ? tetromino.x : tetromino.x + BLOCKSIZE; */
				/* change = true; */
				right_counter = right_counter == 0 ? 6 : 2;
				right_to = right_counter;
			} else {
				right_to--;
			}
		} else
			right_counter = 0;

		if (input.bumper_left && !prev_input.bumper_left && state == PLAY) {
			piece temp = hold;
			hold = tetromino.type;
			if (temp == -1)
				generate_piece();
			else {
				tetromino.type = temp;
				set_blocks();
			}
		}

		if (frame % 10 == 0 && state == PLAY) {
			check_if_sitting();
			tetromino.y = tetromino.bh == LCD_HEIGHT ? 0 : tetromino.y + BLOCKSIZE;
			if (tetromino.bh == LCD_HEIGHT)
				generate_piece();
			change = true;
		}

		if (change) {
			move_blocks();

			/* printf("x: %d y: %d bh: %d bw: %d x1: %d y1: %d\n", tetromino.x, tetromino.y, tetromino.bh, tetromino.bw, tetromino.blocks[0].y, tetromino.blocks[0].x); */
			memset(framebuffer, 0, sizeof(framebuffer));
			for (int i = 0; i < 4; i++) {
				drawSprite(*tetromino.blocks[i], framebuffer);
			}
			for (int i = 0; i < 15; i++)
				for (int j = 0; j < 10; j++)
					if (board[i][j] != NULL) {
						drawSprite(*board[i][j], framebuffer);
					}
			Line border = {10*BLOCKSIZE, 0, 10*BLOCKSIZE, LCD_HEIGHT-1, 0xFFFE};
			drawLine(border, framebuffer);

			change = false;
		}

		frameDraw(framebuffer);

		frame = frame >= 29 ? 0 : frame + 1;
		/* printf("frame: %2d, time: %lld, timer: %lld, delta: %lld\n", frame, time, esp_timer_get_time(), esp_timer_get_time() - time); fflush(stdout); */
		/* vTaskDelay((16666 - (esp_timer_get_time() - time)) / portTICK_PERIOD_MS); */
		long long int deltatime = 33333 - esp_timer_get_time() + time;
		vTaskDelay(deltatime > 0 ? deltatime / 1000 : 0 / portTICK_PERIOD_MS);
	}
}
