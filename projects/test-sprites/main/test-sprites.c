#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_timer.h>

#include "display.h"
#include "input.h"
#include "macros.h"
#include "sprite.h"
#include "array.h"
#include "text.h"

#include "graphics.h"

#define TASKNAME "main"

static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];

enum gamestate{
    PLAYING,
	WINSCREEN,
} state = PLAYING;


int clamp(int a, int min, int max)
{
    return a > min ? (a < max ? a : max) : min;
}


void app_main(void)
{
    // log starting message
    ESP_LOGI(TASKNAME, "Hello, starting up");
    setupDisplay();
    setupInput();

	Input input;

    Sprite playera = {20, 20, 16, 16, playera_sprite, 1, 0, 0, 0};
    Sprite playerb = {284, 20, 16, 16, playerb_sprite, 1, 0, 0, 0};

    bool jumpinga = true,
		 jumpingb = true;
	int coin_counta = 0,
		coin_countb = 0;

	char strbuffer[128] = "";

	const Sprite coin_const[] = {
	    {132, 180, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {148, 180, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {164, 180, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {180, 180, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {68,  132, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {84,  132, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {100, 132, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {116, 132, 8, 8, coin_sprite, 0, 0, 0, 0},
	    {196, 84,  8, 8, coin_sprite, 0, 0, 0, 0},
	    {212, 84,  8, 8, coin_sprite, 0, 0, 0, 0},
	    {228, 84,  8, 8, coin_sprite, 0, 0, 0, 0},
	    {244, 84,  8, 8, coin_sprite, 0, 0, 0, 0},
    };

	Array coins;
	initArray(&coins, 15);
	for (int i = 0; i < ARRAY_COUNT(coin_const); i++)
		insertArray(&coins, coin_const[i]);

	Sprite platform[] = {
		{0,   240, 16, 16, bricks_sprite, 1, 0, 0, 0}, // start invisible blocks
		{16,  240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{32,  240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{48,  240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{64,  240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{80,  240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{96,  240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{112, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{128, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{144, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{160, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{176, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{192, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{208, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{224, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{240, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{256, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{272, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{288, 240, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{304, 240, 16, 16, bricks_sprite, 1, 0, 0, 0}, // end
        {128, 192, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{144, 192, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{160, 192, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{176, 192, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{64,  144, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{80,  144, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{96,  144, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{112, 144, 16, 16, bricks_sprite, 1, 0, 0, 0},
        {192,  96, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{208,  96, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{224,  96, 16, 16, bricks_sprite, 1, 0, 0, 0},
		{240,  96, 16, 16, bricks_sprite, 1, 0, 0, 0},
	};

    int gravity = 2,
		velxa = 0,
		velya = 0,
		velxb = 0,
		velyb = 0;

    float friction = 0.6;

    long long int time,
				  input_time;

    while (true) {
        time = esp_timer_get_time();

			input_time = esp_timer_get_time();
			input = pollInput();
			ESP_LOGI(TASKNAME, "Polling input took %lld μs", ( esp_timer_get_time() - input_time ));

			if (state == PLAYING) {
				if (playera.isOnCeiling)
					velya = 0;
				if (playera.isOnFloor) {
					jumpinga = false;
					velya = 0;
				}

				if (playerb.isOnCeiling)
					velyb = 0;
				if (playerb.isOnFloor) {
					jumpingb = false;
					velyb = 0;
				}

				// Input evaluation
				if (input.start)
					esp_restart();
				if (input.dpad_up && !jumpinga && playera.isOnFloor) {
					jumpinga = true;
					velya -= playera.height + 1;
				}
				if (input.dpad_right)
					velxa += 5;
				if (input.dpad_left)
					velxa -= 5;

				if (input.y && !jumpingb && playerb.isOnFloor) {
					jumpingb = true;
					velyb -= playerb.height + 1;
				}
				if (input.a)
					velxb += 5;
				if (input.x)
					velxb -= 5;

				// Move and draw player
				velya += gravity;
				velxa *= friction;
				velyb += gravity;
				velxb *= friction;

				moveSprite(&playera, velxa, velya, platform, sizeof(platform)/sizeof(platform[0]), NULL, 0);
				playera.y = clamp(playera.y, 0, LCD_HEIGHT - playera.height);
				playera.x = clamp(playera.x, 0, LCD_WIDTH - playera.width);
				moveSprite(&playerb, velxb, velyb, platform, sizeof(platform)/sizeof(platform[0]), NULL, 0);
				playerb.y = clamp(playerb.y, 0, LCD_HEIGHT - playerb.height);
				playerb.x = clamp(playerb.x, 0, LCD_WIDTH - playerb.width);
			} else {
				if (input.start)
					/* state = PLAYING; */
					esp_restart();
			}
			/*printf("floor: %d, vely: %d, y: %d, up: %d, jumping: %d\n", player.isOnFloor, vely, player.y, input.dpad_up, jumping);*/

			for (int i = 0; i < coins.used; i++) {
				if (checkCollision(&playera, &coins.array[i])) {
					removeArray(&coins, i);
					coin_counta++;
				}
			}
			for (int i = 0; i < coins.used; i++) {
				if (checkCollision(&playerb, &coins.array[i])) {
					removeArray(&coins, i);
					coin_countb++;
				}
			}
			if (coins.used == 0)
				state = WINSCREEN;
			ESP_LOGI(TASKNAME, "Calculation time: %lld ms", (esp_timer_get_time() - time) / 1000);

			//Draw background
			memcpy(framebuffer, background_texture, sizeof(background_texture));

			drawSprite(playera, framebuffer);
			drawSprite(playerb, framebuffer);

			for (int i = 19; i < ARRAY_COUNT(platform); i++)
				drawSprite(platform[i], framebuffer);

			for (int i = 0; i < coins.used; i++)
				drawSprite(coins.array[i], framebuffer);

			sprintf(strbuffer, "Red:%d", coin_counta);
			drawText(20, 10, 2, 0xFFFF, strbuffer, framebuffer);
			sprintf(strbuffer, "Blue:%d", coin_countb);
			drawText(230, 10, 2, 0xFFFF, strbuffer, framebuffer);
			if (state == WINSCREEN) {
				if (coin_counta > coin_countb)
					drawText(50, 80, 4, 0x00F8, "Red won!", framebuffer);
				else if (coin_counta < coin_countb)
					drawText(50, 80, 4, 0x1F00, "Blue won!", framebuffer);
				else
					drawText(50, 80, 4, 0xFFFF, "Draw!", framebuffer);
				drawText(50, 150, 2, 0xFFFF, "Press start...", framebuffer);
			}
            frameDraw(framebuffer);

            // Log frame times
			ESP_LOGI(TASKNAME, "Frame time: %lld ms", (esp_timer_get_time() - time) / 1000);
    }

    // Should never get here
    ESP_LOGI(TASKNAME, "Something went HORRIBLY wrong (aka the game exited the main loop).");
    esp_restart();
}
