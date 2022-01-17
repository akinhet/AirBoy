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

    Sprite player = {152, 20, 16, 16, sprite, 1};
    bool jumping = false;

	Sprite platform[3] = {
		{140, 220, 16, 16, sprite, 1},
		{156, 220, 16, 16, sprite, 1},
		{172, 220, 16, 16, sprite, 1},
	};

    int gravity = 2,
		velx = 0,
		vely = 0;

    float friction = 0.9;
		  /*velx = 0,*/
		  /*vely = 0;*/

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
            vely *= friction;

            /*if ( (float) player.y + vely < 0 || (float) player.y + (float) player.height + vely > LCD_HEIGHT ) {*/
                /*vely = 0;*/
                /*jumping = false;*/
            /*}*/
            /*if ( (float) player.x + velx < 0 || (float) player.x + (float) player.width + velx > LCD_WIDTH )*/
                /*velx = 0;*/
            if ( player.y + vely < 0 || player.y + player.height + vely > LCD_HEIGHT ) {
                vely = 0;
                jumping = false;
            }
            if ( player.x + velx < 0 || player.x + player.width + velx > LCD_WIDTH )
                velx = 0;

            /*moveSprite(player, (int) round(velx), (int) round(vely), NULL, 0, NULL, 0); // TODO: Sprite not moving (because of no arrays???)*/
			moveSprite(&player, velx, vely, platform, 3, NULL, 0); // TODO: Sprite not moving (because of no arrays???)
			/*player.x += velx; */
			/*player.y += vely; */
            drawSprite(player, frameBuffer);
			drawSprite(platform[0], frameBuffer);
			drawSprite(platform[1], frameBuffer);
			drawSprite(platform[2], frameBuffer);
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
