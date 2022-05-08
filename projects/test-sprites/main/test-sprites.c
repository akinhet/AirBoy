#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_timer.h>

#include "lib/display.h"
#include "lib/input.h"
#include "lib/macros.h"
#include "lib/sprite.h"
#include "lib/array.h"

#include "graphics.h"

#define TASKNAME "main"

static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];


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
	input.dpad_up = 1;
	input.dpad_left = 1;

    const uint16_t background = SWAP_ENDIAN_16(RGB565(0x4C, 0x7F, 0xFF));

    Sprite player = {152, 20, 16, 16, player_sprite, 1, 0, 0, 0};
    /* Sprite player = {152, 20, 7, 14, player_sprite, 1, 0, 0, 0}; */
    bool jumping = true;
	int coin_count = 0;

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
		velx = 0,
		vely = 0;

    float friction = 0.6;

    long long int time,
				  input_time;

    while (true) {
        time = esp_timer_get_time();

			input_time = esp_timer_get_time();
			input = pollInput();
			ESP_LOGI(TASKNAME, "Polling input took %lld μs", ( esp_timer_get_time() - input_time ));

            if (player.isOnCeiling)
                vely = 0;

            if (player.isOnFloor) {
                jumping = false;
                vely = 0;
            }

            // Input evaluation
			if (input.dpad_up && !jumping && player.isOnFloor) {
				jumping = true;
				vely -= player.height + 1;
			}
			if (input.dpad_right || input.bumper_right)
				velx += 5;
			if (input.dpad_left || input.bumper_left)
				velx -= 5;

            // Draw background
            for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
                framebuffer[i] = background;

            // Move and draw player
            vely += gravity;
            velx *= friction;

			moveSprite(&player, velx, vely, platform, sizeof(platform)/sizeof(platform[0]), NULL, 0);
			player.y = clamp(player.y, 0, LCD_HEIGHT - player.height); 
            player.x = clamp(player.x, 0, LCD_WIDTH - player.width);

			/*printf("floor: %d, vely: %d, y: %d, up: %d, jumping: %d\n", player.isOnFloor, vely, player.y, input.dpad_up, jumping);*/

			for (int i = 0; i < coins.used; i++) {
				if (checkCollision(&player, &coins.array[i])) {
					removeArray(&coins, i);
					coin_count++;
				}
			}
			ESP_LOGI(TASKNAME, "Calculation time: %lld ms", (esp_timer_get_time() - time) / 1000);

			drawSprite(player, framebuffer);

			for (int i = 19; i < ARRAY_COUNT(platform); i++)
				drawSprite(platform[i], framebuffer);

			for (int i = 0; i < coins.used; i++)
				drawSprite(coins.array[i], framebuffer);

            frameDraw(framebuffer);

            // Log frame times
			ESP_LOGI(TASKNAME, "Frame time: %lld ms", (esp_timer_get_time() - time) / 1000);
    }

    // Should never get here
    ESP_LOGI(TASKNAME, "Something went HORRIBLY wrong (aka the game exited the main loop).");
    esp_restart();
}
