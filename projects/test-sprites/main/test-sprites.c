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

#define TASKNAME "main"

static uint16_t frameBuffer[LCD_WIDTH * LCD_HEIGHT];


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

    uint16_t background = SWAP_ENDIAN_16(RGB565(0x4C, 0x7F, 0xFF));

	uint16_t red = SWAP_ENDIAN_16(RGB565(0xFF, 0, 0));
	uint16_t white = 0xFFFF;

	/*uint16_t sprite[256] = {0xFFFF};*/
	uint16_t sprite[256] = {white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red,
							white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red,
							white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red,
							white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red,
							white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red,
							white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red,
							white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red,
							white, red, white, white, white, red, white, white, white, red, white, white, white, red, white, white,
							white, white, white, red, white, white, white, red, white, white, white, red, white, white, white, red };

    Sprite player = {152, 20, 16, 16, sprite, 1, 0, 0, 0};
    bool jumping = true;

	Sprite platform[] = {
        {128, 192, 16, 16, sprite, 1, 0, 0, 0},
		{144, 192, 16, 16, sprite, 1, 0, 0, 0},
		{160, 192, 16, 16, sprite, 1, 0, 0, 0},
		{176, 192, 16, 16, sprite, 1, 0, 0, 0},
		{64,  144, 16, 16, sprite, 1, 0, 0, 0},
		{80,  144, 16, 16, sprite, 1, 0, 0, 0},
		{96,  144, 16, 16, sprite, 1, 0, 0, 0},
		{112, 144, 16, 16, sprite, 1, 0, 0, 0},
        {192,  96, 16, 16, sprite, 1, 0, 0, 0},
		{208,  96, 16, 16, sprite, 1, 0, 0, 0},
		{224,  96, 16, 16, sprite, 1, 0, 0, 0},
		{240,  96, 16, 16, sprite, 1, 0, 0, 0},
		{0,   240, 16, 16, sprite, 1, 0, 0, 0},
		{16,  240, 16, 16, sprite, 1, 0, 0, 0},
		{32,  240, 16, 16, sprite, 1, 0, 0, 0},
		{48,  240, 16, 16, sprite, 1, 0, 0, 0},
		{64,  240, 16, 16, sprite, 1, 0, 0, 0},
		{80,  240, 16, 16, sprite, 1, 0, 0, 0},
		{96,  240, 16, 16, sprite, 1, 0, 0, 0},
		{112, 240, 16, 16, sprite, 1, 0, 0, 0},
		{128, 240, 16, 16, sprite, 1, 0, 0, 0},
		{144, 240, 16, 16, sprite, 1, 0, 0, 0},
		{160, 240, 16, 16, sprite, 1, 0, 0, 0},
		{176, 240, 16, 16, sprite, 1, 0, 0, 0},
		{192, 240, 16, 16, sprite, 1, 0, 0, 0},
		{208, 240, 16, 16, sprite, 1, 0, 0, 0},
		{224, 240, 16, 16, sprite, 1, 0, 0, 0},
		{240, 240, 16, 16, sprite, 1, 0, 0, 0},
		{256, 240, 16, 16, sprite, 1, 0, 0, 0},
		{272, 240, 16, 16, sprite, 1, 0, 0, 0},
		{288, 240, 16, 16, sprite, 1, 0, 0, 0},
		{304, 240, 16, 16, sprite, 1, 0, 0, 0},
	};

    int gravity = 2,
		velx = 0,
		vely = 0;

    float friction = 0.6;

    long long int oldTime = 0,
                  currentTime;

    while (true) {
        currentTime = esp_timer_get_time();
        if (currentTime - oldTime > 16000) {

            input = pollInput();

            // Input evaluation
            if (input.dpad_up && !jumping) {
                jumping = true;
                vely -= player.height + 1;
            }
            if (input.dpad_right)
                velx += 5;
            if (input.dpad_left)
                velx -= 5;

            // Draw background
            for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
                frameBuffer[i] = background;

            // Move and draw player
            vely += gravity;
            velx *= friction;

            if (player.isOnFloor) {
                jumping = false;
                vely = 0;
            }

            if (player.isOnCeiling)
                vely = 0;

			moveSprite(&player, velx, vely, platform, sizeof(platform)/sizeof(platform[0]), NULL, 0);
            /* player.y = clamp(player.y, 0, LCD_HEIGHT - player.height); */
            player.x = clamp(player.x, 0, LCD_WIDTH - player.width);
            drawSprite(player, frameBuffer);
			drawSprite(platform[0], frameBuffer);
			drawSprite(platform[1], frameBuffer);
			drawSprite(platform[2], frameBuffer);
			drawSprite(platform[3], frameBuffer);
			drawSprite(platform[4], frameBuffer);
			drawSprite(platform[5], frameBuffer);
			drawSprite(platform[6], frameBuffer);
			drawSprite(platform[7], frameBuffer);
			drawSprite(platform[8], frameBuffer);
			drawSprite(platform[9], frameBuffer);
			drawSprite(platform[10], frameBuffer);
			drawSprite(platform[11], frameBuffer);
            frameDraw(frameBuffer);

            // Log frame times
            ESP_LOGI(TASKNAME, "Frame time: %lld ms", (currentTime - oldTime) / 1000);
            oldTime = currentTime;
        }
    }

    // Should never get here
    ESP_LOGI(TASKNAME, "Something went HORRIBLY wrong (aka the game exited the main loop).");
    esp_restart();
}
