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
#include "sprite.h"
#include "array.h"
#include "textures.h"

#define TASKNAME "main"
#define screenWidth 320
#define screenHeight 180
#define mapWidth 24
#define mapHeight 24
#define BGCOLORU 0x3F1C
#define BGCOLORD 0x6529
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
    // log starting message
    ESP_LOGI(TASKNAME, "Hello, starting up");
    setupDisplay();
    setupInput();

	float posX = 18, posY = 6;  //x and y start position
	float dirX = -1, dirY = 0; //initial direction vector
	float planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

	int timeF = esp_timer_get_time(); //time of current frame
	int oldTime = 0; //time of previous frame
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

    //const uint16_t background = SWAP_ENDIAN_16(RGB565(0x4C, 0x7F, 0xFF));

    //Sprite player = {152, 20, 16, 16, player_sprite, 1, 0, 0, 0};
    /* Sprite player = {152, 20, 7, 14, player_sprite, 1, 0, 0, 0}; */
    //bool jumping = true;
	//int coin_count = 0;

	/*const Sprite coin_const[] = {
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
		insertArray(&coins, coin_const[i]);*/

	/*Sprite platform[] = {
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

    float friction = 0.6;*/

    long long int time,
				  input_time;

	//esp_task_wdt_init(30, false);

    for(;;) 
	{
        time = esp_timer_get_time();
		input = pollInput();

		for(int x = 0; x < w; x++)
		{
			//calculate ray position and direction
			float cameraX = 2 * x / (float)(w) - 1; //x-coordinate in camera space
			float rayDirX = dirX + planeX * cameraX;
			float rayDirY = dirY + planeY * cameraX;
			//which box of the map we're in
			int mapX = (int)posX;
			int mapY = (int)posY;

			//length of ray from current position to next x or y-side
			float sideDistX;
			float sideDistY;

			//length of ray from one x or y-side to next x or y-side
			//these are derived as:
			//deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
			//deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
			//which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
			//where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
			//unlike (dirX, dirY) is not 1, however this does not matter, only the
			//ratio between deltaDistX and deltaDistY matters, due to the way the DDA
			//stepping further below works. So the values can be computed as below.
			// Division through zero is prevented, even though technically that's not
			// needed in C++ with IEEE 754 floating point values.
			float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
			float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

			//ESP_LOGI(TASKNAME, "zmienna wartosc %09.6f w x = %d", rayDirX, x);
			//printf("czy dziala     %.6f      at     x= %d", deltaDistX, x);

			float perpWallDist;

			//what direction to step in x or y-direction (either +1 or -1)
			int stepX;
			int stepY;

			int hit = 0; //was there a wall hit?
			int side = 0; //was a NS or a EW wall hit?
			//calculate step and initial sideDist
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
			//perform DDA
			while(hit == 0)
			{
				//jump to next map square, either in x-direction, or in y-direction
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

			//Calculate height of line to draw on screen
			int lineHeight = (int)(h / perpWallDist);

			int pitch = 100;

			int drawStart = -(lineHeight >> 1) + (h >> 1); //+ pitch;
			if(drawStart < 0) drawStart = 0;
			int drawEnd = (lineHeight >> 1) + (h >> 1); //+ pitch;
			if(drawEnd >= h) drawEnd = h - 1;

			int texNum = worldMap[mapX][mapY] - 1;

			float wallX; //where exactly the wall was hit
			if (side == 0) wallX = posY + perpWallDist * rayDirY;
			else           wallX = posX + perpWallDist * rayDirX;
			wallX -= floor(wallX);

			//x coordinate on the texture
			int texX = (int) (wallX * (float) texWidth);
			if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
			if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

			float stepW = 1.0 * texHeight / lineHeight;
			float texPos = (drawStart  - (h >> 1) + (lineHeight >> 1)) * stepW;

			/*uint16_t color;
			switch(worldMap[mapX][mapY])
			{
				case 1:  color = 0XF000;    break; //red
				case 2:  color = 0XF00F;  break; //green
				case 3:  color = 0X0FF0;   break; //blue
				case 4:  color = 0XFFFF;  break; //white
				default: color = 0XFF00; break; //yellow
			}*/

			//give x and y sides different brightness
			//if(side == 1) color = color >> 2;


			/*for (uint32_t y = 0; y < 180; y++)
			{
				if (y < drawStart)
				{
				framebuffer[320 * y + x] = BGCOLORU;
				}
				else if (y < drawEnd)
				{
				framebuffer[320 * y + x] = color;
				}else{
				framebuffer[320 * y + x] = BGCOLORD;
				}
			}*/

			for (uint32_t y = 0; y < 180; y++)
			{
				if (y < drawStart)
				{
				framebuffer[320 * y + x] = BGCOLORU;
				}
				else if (y < drawEnd)
				{
				int texY = (int)texPos & (texHeight - 1);
				texPos += stepW;
				uint16_t color = texture[texNum][texHeight * texY + texX];
				framebuffer[320 * y + x] = SWAP_ENDIAN_16(color);
				}else{
				framebuffer[320 * y + x] = BGCOLORD;
				}
			}
		}
		//ESP_LOGI(TASKNAME, "Calculation time: %lld ms", (esp_timer_get_time() - time) / 1000);

        frameDraw(framebuffer);


		//for (int i = 0; i < 240 * 320; i++)
		//{
		//	framebuffer[i] = 0;
		//}

		oldTime = timeF;
   		timeF = esp_timer_get_time();
    	frameTime = (timeF - oldTime) / 10000;
		//ESP_LOGI(TASKNAME, "Frame time: %d ms", frameTime);

		float moveSpeed = (float)frameTime * 0.1f; //the constant value is in squares/second
		float rotSpeed = (float)frameTime * 0.05f; //the constant value is in radians/second
		

		//move forward if no wall in front of you
		if(input.dpad_up)
		{
			if(worldMap[(int) (posX + dirX * moveSpeed)][(int) posY] == false) posX += dirX * moveSpeed;
			if(worldMap[(int) posX][(int) (posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
		}

		//Serial.println(dirX);
		//Serial.println(dirY);
		//move backwards if no wall behind you
		if(input.dpad_down)
		{
			if(worldMap[(int) (posX - dirX * moveSpeed)][(int) posY] == false) posX -= dirX * moveSpeed;
			if(worldMap[(int) posX][(int) (posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
		}
		//rotate to the right
		if(input.dpad_right || input.bumper_right)
		{
			//both camera direction and camera plane must be rotated
			float oldDirX = dirX;
			dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
			float oldPlaneX = planeX;
			planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
		}
		//rotate to the left
		if(input.dpad_left || input.bumper_left)
		{
			//both camera direction and camera plane must be rotated
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

    // Should never get here
    ESP_LOGI(TASKNAME, "Something went HORRIBLY wrong (aka the game exited the main loop).");
    esp_restart();
}
