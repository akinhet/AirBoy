#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_timer.h>
#include <esp_task_wdt.h>

#include "display.h"
#include "input.h"
#include "macros.h"
#include "textures.h"

#define TASKNAME "main"

#define mapWidth 8
#define mapHeight 8
#define mapSize 64

#define texWidth 64
#define texHeight 64

Input input;
static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];

const int map[] = {
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,1,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};

Rectangle tile = {0, 0, 14, 14, 0xe739};

const Rectangle roof = {0, 0, LCD_WIDTH, LCD_HEIGHT, 0xE318};

Rectangle player = {0, 0, 8, 8, 0xe0ff};
Line playerdir = {0, 0, 0, 0, 0xe0ff};
float playerx = 16,
	  playery = 16,
	  playerdeltax = 0,
	  playerdeltay = 0,
	  playerangle = 0;

Line ray = {0, 0, 0, 0, 0xe007};


void draw_player()
{
	player.x = (int) playerx;
	player.y = (int) playery;
	playerdir.x0 = (int) playerx;
	playerdir.y0 = (int) playery;
	playerdir.x1 = (int) playerx + playerdeltax * 10;
	playerdir.y1 = (int) playery + playerdeltay * 10;
	drawLine(playerdir, framebuffer);
	drawRect(player, framebuffer);
}


void handle_input()
{
	input = pollInput();
	if (input.dpad_right) {
		playerangle += 0.1;
		if (playerangle > 2*M_PI)
			playerangle -= 2*M_PI;
		playerdeltax = cos(playerangle);
		playerdeltay = sin(playerangle);
	}
	if (input.dpad_left) {
		playerangle -= 0.1;
		if (playerangle < 0)
			playerangle += 2*M_PI;
		playerdeltax = cos(playerangle);
		playerdeltay = sin(playerangle);
	}
	if (input.dpad_up) {
		playerx += playerdeltax;
		playery += playerdeltay;
	}
	if (input.dpad_down) {
		playerx -= playerdeltax;
		playery -= playerdeltay;
	}
}


void draw_2dmap()
{
	for (int y = 0; y < mapHeight; y++)
		for (int x = 0; x < mapWidth; x++) {
			if (map[y*mapWidth+x] == 1) {
				tile.x = x * 16;
				tile.y = y * 16;
				drawRect(tile, framebuffer);
			}
		}
}


float distance2D(float x0, float y0, float x1, float y1)
{
	float dx = x1 - x0,
		dy = y1 - y0;
	return sqrt(dx*dx + dy*dy);
}


void draw_rays_3d()
{
	float rayangle = 0,
		rayy = playery,
		rayx = playerx,
		yoffset = 0,
		xoffset = 0,
		distance = 0,
		onedegree = 1 * M_PI / 180;
	int maph = 0,
		mapv = 0;

	rayangle = playerangle - onedegree*30;
	if (rayangle < 0)
		rayangle += 2*M_PI;
	if (rayangle > 2*M_PI)
		rayangle -= 2*M_PI;

	for (int i = 0; i < 60; i++) {
		// horizontal
		int dof = 0;
		float atan = -1/tan(rayangle),
			disH = 10000000,
			horizontalx = playerx,
			horizontaly = playery;

		if (rayangle > M_PI) {
			rayy = (((int) playery >> 4) << 4) - 0.0001;
			rayx = (playery - rayy) * atan + playerx;
			yoffset = -16;
			xoffset = -yoffset * atan;
		}
		if (rayangle < M_PI) {
			rayy = (((int) playery >> 4) << 4) + 16;
			rayx = (playery - rayy) * atan + playerx;
			yoffset = 16;
			xoffset = -yoffset * atan;
		}
		if (rayangle == 0 || rayangle == (float) M_PI) {
			rayx = playerx;
			rayy = playery;
			dof = 8;
		}

		while (dof < 8) {
			int mapx = (int) rayx >> 4,
				mapy = (int) rayy >> 4,
				mapindex = mapy * mapWidth + mapx;

			if (mapindex < mapWidth * mapHeight && mapindex > 0 && map[mapindex] > 0) {
				dof = 8;
				horizontalx = rayx;
				horizontaly = rayy;
				disH = distance2D(playerx, playery, rayx, rayy);
				maph = map[mapindex];
			} else {
				rayx += xoffset;
				rayy += yoffset;
				dof += 1;
			}
		}

		// vertical
		dof = 0;
		float disV = 10000000,
			verticalx = playerx,
			verticaly = playery,
			ntan = -tan(rayangle);

		if (rayangle > M_PI_2 && rayangle < M_PI_2*3) {
			rayx = (((int) playerx >> 4) << 4) - 0.0001;
			rayy = (playerx - rayx) * ntan + playery;
			xoffset = -16;
			yoffset = -xoffset * ntan;
		}
		if (rayangle < M_PI_2 || rayangle > M_PI_2*3) {
			rayx = (((int) playerx >> 4) << 4) + 16;
			rayy = (playerx - rayx) * ntan + playery;
			xoffset = 16;
			yoffset = -xoffset * ntan;
		}
		if (rayangle == (float) M_PI_2 || rayangle == M_PI_2*3) {
			rayx = playerx;
			rayy = playery;
			dof = 8;
		}

		while (dof < 8) {
			int mapx = (int) rayx >> 4,
				mapy = (int) rayy >> 4,
				mapindex = mapy * mapWidth + mapx;

			if (mapindex < mapWidth * mapHeight && mapindex > 0 && map[mapindex] > 0) {
				dof = 8;
				verticalx = rayx;
				verticaly = rayy;
				disV = distance2D(playerx, playery, rayx, rayy);
				mapv = map[mapindex];
			} else {
				rayx += xoffset;
				rayy += yoffset;
				dof += 1;
			}
		}

		if (disH < disV) {
			rayx = horizontalx;
			rayy = horizontaly;
			distance = disH;
		} else {
			distance = disV;
		}

		ray.x0 = playerx;
		ray.y0 = playery;
		ray.x1 = rayx;
		ray.y1 = rayy;
		/* if (rayx >= LCD_WIDTH) */
		/* 	ray.x1 = LCD_WIDTH - 1; */
		/* if (rayx < 0) */
		/* 	ray.x1 = 0; */
		/* if (rayy < 0) */
		/* 	ray.y1 = 0; */
		/* if (rayy >= LCD_HEIGHT) */
		/* 	ray.y1 = LCD_HEIGHT - 1; */
		ESP_LOGI(TASKNAME, "x0: %d, y0: %d, x1: %d, y1: %d", ray.x0,ray.y0,ray.x1,ray.y1);
		/* if (rayx < LCD_WIDTH && rayx >= 0 && rayy < LCD_HEIGHT && rayy >= 0) */
		/* 	drawLine(ray, framebuffer); */

		// walls
		/* float anglediff = playerangle - rayangle, */
		/* 	lineheight = fminf((mapSize * 320) / (distance * cosf(anglediff)), 320), */
		/* 	lineoffset = 160 - lineheight / 2; */
		/* if (anglediff < 0) */
		/* 	anglediff += 2*M_PI; */
		/* if (anglediff > 2*M_PI) */
		/* 	anglediff -= 2*M_PI; */
		rayangle += onedegree;
		if (rayangle < 0)
			rayangle += 2*M_PI;
		if (rayangle > 2*M_PI)
			rayangle -= 2*M_PI;
	}
}


void app_main(void)
{
	ESP_LOGI(TASKNAME, "Hello, starting up");
	setupDisplay();
	setupInput();

	while (true) {
		memset(framebuffer, 0, sizeof(framebuffer));
		drawRect(roof, framebuffer);
		draw_2dmap();

		handle_input();
		draw_player();
		draw_rays_3d();

		frameDraw(framebuffer);
		vTaskDelay(16 / portTICK_PERIOD_MS);
	}

	ESP_LOGI(TASKNAME, "Something went HORRIBLY wrong (aka the game exited the main loop).");
	esp_restart();
}
