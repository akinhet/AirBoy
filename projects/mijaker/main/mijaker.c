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

#define screenWidth 320
#define screenHeight 180

#define mapWidth 24
#define mapHeight 24

#define texWidth 64
#define texHeight 64

Input input;
static uint16_t framebuffer[LCD_WIDTH * LCD_HEIGHT];

int worldMap[mapWidth][mapHeight]=
{
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

void app_main(void)
{
    ESP_LOGI(TASKNAME, "Hello, starting up");
    setupDisplay();
    setupInput();

	float posX = 18, posY = 6; 
	float dirX = -1, dirY = 0;
	float planeX = 0, planeY = 0.66;

	int timeF = esp_timer_get_time();
	int oldTime = 0;
	int frameTime;

	int w = screenWidth;
	int h = screenHeight;

	uint16_t **texture = (uint16_t **)malloc(8 * sizeof(uint16_t*)); 

	for (uint8_t i = 0; i < 8; i++)
	{
		texture[i] = (uint16_t *)malloc(texHeight * texWidth * sizeof(uint16_t)); 
	}

	for(int i = 0; i < 4096; i++)
	{
		texture[0][i] = image_data_bluestone[i];
	}
	for(int i = 0; i < 4096; i++)
	{
		texture[1][i] = image_data_colorstone[i];
	}
	for(int i = 0; i < 4096; i++)
	{
		texture[2][i] = image_data_eagle[i];
	}
	for(int i = 0; i < 4096; i++)
	{
		texture[3][i] = image_data_greystone[i];
	}
	for(int i = 0; i < 4096; i++)
	{
		texture[4][i] = image_data_mossy[i];
	}
	for(int i = 0; i < 4096; i++)
	{
		texture[5][i] = image_data_purplestone[i];
	}
	for(int i = 0; i < 4096; i++)
	{
		texture[6][i] = image_data_redbrick[i];
	}
	for(int i = 0; i < 4096; i++)
	{
		texture[7][i] = image_data_wood[i];;
	}

    for(;;) 
	{
		input = pollInput();

		for(int y = 0; y < h; y++)
		{
			float rayDirX0 = dirX - planeX;
			float rayDirY0 = dirY - planeY;
			float rayDirX1 = dirX + planeX;
			float rayDirY1 = dirY + planeY;

			int p = y - screenHeight / 2;

			float posZ = 0.5 * screenHeight;

			float rowDistance = posZ / p;

			float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
			float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

			float floorX = posX + rowDistance * rayDirX0;
			float floorY = posY + rowDistance * rayDirY0;

			for(int x = 0; x < screenWidth; ++x)
			{
				int cellX = (int)(floorX);
				int cellY = (int)(floorY);

				int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
				int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

				floorX += floorStepX;
				floorY += floorStepY;

				int floorTexture = 3;

				uint16_t color;

				color = texture[floorTexture][texWidth * ty + tx];
				framebuffer[320 * y + x] = SWAP_ENDIAN_16(color - 40);
			}
		}

		for(int x = 0; x < w; x++)
		{

			float cameraX = 2 * x / (float)(w) - 1;
			float rayDirX = dirX + planeX * cameraX;
			float rayDirY = dirY + planeY * cameraX;

			int mapX = (int)posX;
			int mapY = (int)posY;

			float sideDistX;
			float sideDistY;

			float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
			float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

			float perpWallDist;

			int stepX;
			int stepY;

			int hit = 0;
			int side;

			if(rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - posX) * deltaDistX;
			}
			if(rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - posY) * deltaDistY;
			}

			while(hit == 0)
			{
				if(sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}

				if(worldMap[mapX][mapY] > 0) hit = 1;
			}

			if(side == 0) perpWallDist = (sideDistX - deltaDistX);
			else          perpWallDist = (sideDistY - deltaDistY);

			int lineHeight = (int)(h / perpWallDist);

			int drawStart = -(lineHeight >> 1) + (h >> 1);
			if(drawStart < 0) drawStart = 0;
			int drawEnd = (lineHeight >> 1) + (h >> 1);
			if(drawEnd >= h) drawEnd = h;

			int texNum = worldMap[mapX][mapY] - 1;

			float wallX;
			if (side == 0) wallX = posY + perpWallDist * rayDirY;
			else           wallX = posX + perpWallDist * rayDirX;
			wallX -= floor(wallX);

			int texX = (int) (wallX * (float) texWidth);
			if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
			if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

			float stepW = 1.0 * texHeight / lineHeight;
			float texPos = (drawStart  - (h >> 1) + (lineHeight >> 1)) * stepW;

			for (uint8_t y = drawStart; y < drawEnd; y++)
			{
				int texY = (int)texPos & (texHeight - 1);
				texPos += stepW;
			uint16_t color = texture[texNum][texHeight * texY + texX];
				framebuffer[320 * y + x] = SWAP_ENDIAN_16(color);
			}
			
		}

        frameDraw(framebuffer);

		memset(framebuffer, 0, sizeof(framebuffer)); 

		oldTime = timeF;
   		timeF = esp_timer_get_time();
    	frameTime = (timeF - oldTime) / 10000;

		float moveSpeed = (float)frameTime * 0.1f;
		float rotSpeed = (float)frameTime * 0.05f;
		
		if(input.dpad_up)
		{
			if(worldMap[(int) (posX + dirX * moveSpeed)][(int) posY] == false) posX += dirX * moveSpeed;
			if(worldMap[(int) posX][(int) (posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
		}

		if(input.dpad_down)
		{
			if(worldMap[(int) (posX - dirX * moveSpeed)][(int) posY] == false) posX -= dirX * moveSpeed;
			if(worldMap[(int) posX][(int) (posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
		}

		if(input.dpad_right || input.bumper_right)
		{
			float oldDirX = dirX;
			dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
			float oldPlaneX = planeX;
			planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
		}

		if(input.dpad_left || input.bumper_left)
		{
			float oldDirX = dirX;
			dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
			dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
			float oldPlaneX = planeX;
			planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
			planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
		}
		if(input.a)
		{
			dirX *= 0.9;
			dirY *= 0.9;
		}
		if(input.b)
		{
			dirX *= 1.1;
			dirY *= 1.1;
		}
	}

    ESP_LOGI(TASKNAME, "Something went HORRIBLY wrong (aka the game exited the main loop).");
    esp_restart();
}
