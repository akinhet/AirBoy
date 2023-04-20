/*
 * Mijaker 2
 * by Piotr Zieniewicz
 * credits: https://lodev.org/cgtutor/raycasting.html
 */
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
#include "text.h"

#define TASKNAME "main"

#define mapWidth 24
#define mapHeight 24
#define mapSize 64

#define texWidth 64
#define texHeight 64

Input input;
static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];

const int worldmap[mapWidth][mapHeight] =
{
/*
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
*/
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

const int floormap[mapWidth][mapHeight]=
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,6,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,6,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
};


/*
image_data_bluestone
image_data_colorstone
image_data_eagle
image_data_greystone
image_data_mossy
image_data_purplestone
image_data_redbrick
image_data_wood
*/
/* uint16_t textures[8][texHeight * texWidth] = {0}; */
uint16_t *textures[8];

const Rectangle ceiling_tex = {0, 0, LCD_WIDTH, LCD_HEIGHT / 2, 0xB6B5};
const Rectangle floor_tex = {0, LCD_HEIGHT / 2, LCD_WIDTH, LCD_HEIGHT / 2, 0x0842};

float playerx = 22,
	   playery = 12,
	   playerdeltax = 0,
	   playerdeltay = 0,
	   playerdirx = -1,
	   playerdiry = 0,
	   playerleftx = 0,
	   playerlefty = -1,
	   playercplanex = 0,
	   playercplaney = 0.66,
	   playerrotspeed = 0.16 * 3.0,
	   playermovespeed = 0.16 * 5.0;


void load_textures()
{
	for (int i = 0; i < 8; i++)
		textures[i] = malloc(texHeight * texWidth * 2);
	memcpy(textures[0], image_data_bluestone,   texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[1], image_data_colorstone,  texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[2], image_data_eagle,       texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[3], image_data_greystone,   texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[4], image_data_mossy,       texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[5], image_data_purplestone, texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[6], image_data_redbrick,    texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[7], image_data_wood,        texHeight * texWidth * sizeof(uint16_t));
}


void handle_input()
{
	input = pollInput();
	if (input.dpad_right) {
		float oldplayerdirx = playerdirx;
		playerdirx = playerdirx * cos(-playerrotspeed) - playerdiry * sin(-playerrotspeed);
		playerdiry = oldplayerdirx * sin(-playerrotspeed) + playerdiry * cos(-playerrotspeed);
		float oldplayercplanex = playercplanex;
		playercplanex = playercplanex * cos(-playerrotspeed) - playercplaney * sin(-playerrotspeed);
		playercplaney = oldplayercplanex * sin(-playerrotspeed) + playercplaney * cos(-playerrotspeed);

		playerleftx = -playerdiry;
		playerlefty = playerdirx;
	}
	if (input.dpad_left) {
		float oldplayerdirx = playerdirx;
		playerdirx = playerdirx * cos(playerrotspeed) - playerdiry * sin(playerrotspeed);
		playerdiry = oldplayerdirx * sin(playerrotspeed) + playerdiry * cos(playerrotspeed);
		float oldplayercplanex = playercplanex;
		playercplanex = playercplanex * cos(playerrotspeed) - playercplaney * sin(playerrotspeed);
		playercplaney = oldplayercplanex * sin(playerrotspeed) + playercplaney * cos(playerrotspeed);

		playerleftx = -playerdiry;
		playerlefty = playerdirx;
	}
	if (input.bumper_left) {
		if (!worldmap[(int) (playerx + playerleftx * playermovespeed)][(int) playery])
			playerx += playerleftx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery + playerlefty * playermovespeed)])
			playery += playerlefty * playermovespeed;
	}
	if (input.bumper_right) {
		if (!worldmap[(int) (playerx - playerleftx * playermovespeed)][(int) playery])
			playerx -= playerleftx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery - playerlefty * playermovespeed)])
			playery -= playerlefty * playermovespeed;
	}
	if (input.dpad_up) {
		if (!worldmap[(int) (playerx + playerdirx * playermovespeed)][(int) playery])
			playerx += playerdirx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery + playerdiry * playermovespeed)])
			playery += playerdiry * playermovespeed;
	}
	if (input.dpad_down) {
		if (!worldmap[(int) (playerx - playerdirx * playermovespeed)][(int) playery])
			playerx -= playerdirx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery - playerdiry * playermovespeed)])
			playery -= playerdiry * playermovespeed;
	}
}


void draw_player_view()
{
	float raydirx0 = playerdirx - playercplanex,
		  raydiry0 = playerdiry - playercplaney,
		  raydirx1 = playerdirx + playercplanex,
		  raydiry1 = playerdiry + playercplaney;

	for (int y = LCD_HEIGHT / 2 + 1; y < LCD_HEIGHT; y++) {
		int p = y - LCD_HEIGHT / 2;
		float rowdistance = LCD_HEIGHT / (2.0 * p),
			  floorstepx = rowdistance * (raydirx1 - raydirx0) / LCD_WIDTH,
			  floorstepy = rowdistance * (raydiry1 - raydiry0) / LCD_WIDTH,
			  floorx = playerx + rowdistance * raydirx0,
			  floory = playery + rowdistance * raydiry0;

		for (int x = 0; x < LCD_WIDTH; x++) {
			int cellx = (int) floorx,
				celly = (int) floory,
				texx = (int) (texWidth * (floorx - cellx)) & (texWidth - 1),
				texy = (int) (texHeight * (floory - celly)) & (texHeight - 1);

			floorx += floorstepx;
			floory += floorstepy;

			int floortexture = floormap[abs(cellx & (mapWidth - 1))][abs(celly & (mapHeight - 1))],
				ceilingtexture = 6;
			uint16_t pixelcolor;
			/* if (floortexture > 7 || floortexture < 0) */
				/* ESP_LOGI(TASKNAME, "cellx: %d celly: %d", cellx, celly); */

			pixelcolor = SWAP_ENDIAN_16(textures[floortexture][texWidth * texy + texx]);
			pixelcolor = (pixelcolor >> 1) & 0xEF7B;
			framebuffer[y * LCD_WIDTH + x] = pixelcolor;

			pixelcolor = SWAP_ENDIAN_16(textures[ceilingtexture][texWidth * texy + texx]);
			pixelcolor = (pixelcolor >> 1) & 0xEF7B;
			framebuffer[(LCD_HEIGHT - y - 1) * LCD_WIDTH + x] = pixelcolor;
		}
	}

	for (int x = 0; x < LCD_WIDTH; x++) {
		float camerax = 2 * x / (float) LCD_WIDTH - 1,
			   raydirx = playerdirx + playercplanex * camerax,
			   raydiry = playerdiry + playercplaney * camerax,
			   deltadistx = (raydirx == 0) ? 1e30 : fabs(1 / raydirx),
			   deltadisty = (raydiry == 0) ? 1e30 : fabs(1 / raydiry),
			   sidedistx,
			   sidedisty,
			   perpwalldist;

		int mapx = (int) playerx,
			mapy = (int) playery,
			stepx = 0,
			stepy = 0,
			hit = 0,
			side; // 0 = NS, 1 = EW

		if (raydirx < 0) {
			stepx = -1;
			sidedistx = (playerx - mapx) * deltadistx;
		} else {
			stepx = 1;
			sidedistx = (mapx + 1.0 - playerx) * deltadistx;
		}
		if (raydiry < 0) {
			stepy = -1;
			sidedisty = (playery - mapy) * deltadisty;
		} else {
			stepy = 1;
			sidedisty = (mapy + 1.0 - playery) * deltadisty;
		}

		while (!hit) {
			if (sidedistx < sidedisty) {
				sidedistx += deltadistx;
				mapx += stepx;
				side = 0;
			} else {
				sidedisty += deltadisty;
				mapy += stepy;
				side = 1;
			}

			if (worldmap[mapx][mapy] > 0)
				hit = 1;
		}

		if (!side)
			perpwalldist = (sidedistx - deltadistx);
		else
			perpwalldist = (sidedisty - deltadisty);

		int lineheight = (int)(LCD_HEIGHT / perpwalldist),
			linestart = (LCD_HEIGHT - lineheight) / 2,
			lineend = (LCD_HEIGHT + lineheight) / 2;

		if (linestart < 0)
			linestart = 0;
		if (lineend >= LCD_HEIGHT)
			lineend = LCD_HEIGHT - 1;

		/* uint16_t *texture = (uint16_t *)malloc(texHeight * texWidth * sizeof(uint16_t)); */
		/* if (texture == NULL) */
		/* 	ESP_LOGI(TASKNAME, "dupa"); */
		/* switch (worldmap[mapx][mapy]) { */
		/* 	default: memcpy(texture, image_data_bluestone,   texHeight * texWidth * sizeof(uint16_t)); break; */
		/* } */
		int texindex = worldmap[mapx][mapy] - 1;

		float wallx = (!side) ?
			playery + perpwalldist * raydiry :
			playerx + perpwalldist * raydirx;
		wallx -= floor(wallx);

		int texx = (int) (wallx * (float) texWidth);
		if (!side && raydirx > 0)
			texx = texWidth - texx - 1;
		if (side && raydiry < 0)
			texx = texWidth - texx - 1;

		float step = 1.0 * texHeight / lineheight,
			   texposition = (linestart - LCD_HEIGHT / 2 + lineheight / 2) * step; // TODO: check what pitch does

		for (int y = linestart; y < lineend; y++) {
			int texy = (int) texposition & (texHeight - 1);
			texposition += step;

			uint16_t pixelcolor = SWAP_ENDIAN_16(textures[texindex][texHeight * texy + texx]);
			if (side)
				pixelcolor = (pixelcolor >> 1) & 0xEF7B;

			framebuffer[LCD_WIDTH * y + x] = pixelcolor;
		}
		/* free(texture); */

		/* flat color renderer
		uint16_t wallcolor;
		switch (worldmap[mapx][mapy]) {
			case 1:  wallcolor = 0x00F8; break; // red
			case 2:  wallcolor = 0xE007; break; // green
			case 3:  wallcolor = 0x1F00; break; // blue
			case 4:  wallcolor = 0xFFFE; break; // white
			default: wallcolor = 0xE0FF; break; // yellow
		}

		if (side)
			//wallcolor = (wallcolor & 0x00F8 >> 1 & 0x00F8) | (wallcolor & 0xE007 >> 1 & 0xE007) | (wallcolor & 0x1F00 >> 1 & 0x1F00);
			wallcolor = (wallcolor >> 1) & 0xEF7B;

		Line line = {x, linestart, x, lineend, wallcolor};
		drawLine(line, framebuffer);
		*/
	}
}


void app_main(void)
{
	ESP_LOGI(TASKNAME, "Hello, starting up");
	setupDisplay();
	setupInput();
	load_textures();
	uint64_t time = 0,
		oldtime = 0;
	float delta = 0;
	char strbuf[32] = "";

	while (true) {
		memset(framebuffer, 0, sizeof(framebuffer));
		drawRect(floor_tex, framebuffer);
		drawRect(ceiling_tex, framebuffer);
		draw_player_view();

		playermovespeed = delta * 6.0;
		playerrotspeed = delta * 3.0;
		handle_input();

		sprintf(strbuf, "%d", (int) (1.0 / delta));
		drawText(0, 0, 3, 0xFFFE, strbuf, framebuffer);

		frameDraw(framebuffer);
		oldtime = time;
		time = esp_timer_get_time();
		delta = (time - oldtime) / 1000000.0;
		ESP_LOGI(TASKNAME, "%f", 1.0 / delta);		// FIXME: work on the fps to get a stable 20
		/* vTaskDelay(delta > 16 ? 1 : (int) (16 - delta) / portTICK_PERIOD_MS); */
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}

	ESP_LOGI(TASKNAME, "Something went HORRIBLY wrong (aka the game exited the main loop).");
	esp_restart();
}
