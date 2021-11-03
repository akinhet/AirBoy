#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_DEPTH 2
#define TASKNAME "main"

#define BYTES_TO_BITS(value)		( (value) * 8 )
#define ARRAY_COUNT(value)			( sizeof(value) / sizeof(value[0]) )
#define UPPER_BYTE_16(value)		( (value) >> 8u )
#define LOWER_BYTE_16(value)		( (value) & 0xFFu )
#define RGB565(red, green, blue)	( (((red) >> 3u) << 11u) | (((green) >> 2u) << 5u) | ((blue) >> 3u))
#define SWAP_ENDIAN_16(value)		( (((value) & 0xFFu) << 8u) | ((value) >> 8u) )

enum pins{
	LCD_PIN_MISO	= 19,
	LCD_PIN_MOSI	= 23,
	LCD_PIN_SCLK	= 18,
	LCD_PIN_CS		= 15,
	LCD_PIN_DC		=  2,
	LCD_PIN_RESET	=  4,
};

typedef enum{
	SOFTWARE_RESET				= 0x01u,
	DISPLAY_OFF					= 0x28u,
	DISPLAY_ON					= 0x29u,
	ENTER_SLEEP_MODE			= 0x10u,
	SLEEP_OUT					= 0x11u,
	DISPLAY_INVERSION_OFF		= 0x20u,
	DISPLAY_INVERSION_ON		= 0x21u,
	COLUMN_ADDRESS_SET			= 0x2Au,
	PAGE_ADDRESS_SET			= 0x2Bu,
	MEMORY_WRITE				= 0x2Cu,
	MEMORY_ACCESS_CONTROL		= 0x36u,
	PIXEL_FORMAT_SET			= 0x3Au,
	FRAME_RATE_CONTROL_1		= 0xB1u,
	DISPLAY_FUNCTION_CONTROL	= 0xB6u,
	POWER_CONTROL_1				= 0xC0u,
	POWER_CONTROL_2				= 0xC1u,
	VCOM_CONTROL_1				= 0xC5u,
	VCOM_CONTROL_2				= 0xC7u,
} CommandCode;

typedef struct{
	CommandCode code;
	uint8_t parameters[15];
	uint8_t length;
	int delay;
} Command;

static uint16_t frameBuffer[LCD_WIDTH * LCD_HEIGHT];


// https://github.com/adafruit/Adafruit_ILI9341/blob/master/Adafruit_ILI9341.cpp#L151
Command StartupCommands[] = {
	{ 
		SOFTWARE_RESET, 
		{}, 
		0,
		150
	},
	{
		0xCF,
		{0x00, 0xC1, 0x30},
		3,
		5
	},
	/*{*/
		/*0xED,*/
		/*{0x64, 0x03, 0x12, 0x81},*/
		/*4,*/
		/*5*/
	/*},*/
	/*{*/
		/*0xE8,*/
		/*{0x85, 0x00, 0x78},*/
		/*3,*/
		/*5*/
	/*},*/
	/*{*/
		/*0xCB,*/
		/*{0x39, 0x2C, 0x00, 0x34, 0x02},*/
		/*5,*/
		/*5*/
	/*},*/
	/*{*/
		/*0xF7,*/
		/*{0x20},*/
		/*1,*/
		/*5*/
	/*},*/
	/*{*/
		/*0xEA,*/
		/*{0x00, 0x00},*/
		/*2,*/
		/*5*/
	/*},*/
	/*{*/
		/*POWER_CONTROL_1,*/
		/*{0x10},*/
		/*1,*/
		/*5*/
	/*},*/
	/*{*/
		/*POWER_CONTROL_2,*/
		/*{0x00},*/
		/*1,*/
		/*5*/
	/*},*/
	/*{*/
		/*VCOM_CONTROL_1,*/
		/*{0x30, 0x30},*/
		/*2,*/
		/*5*/
	/*},*/
	/*{*/
		/*VCOM_CONTROL_2,*/
		/*{0xB7},*/
		/*1,*/
		/*5*/
	/*},*/
	{
		PIXEL_FORMAT_SET,
		{0x55},
		1,
		5
	},
	{
		MEMORY_ACCESS_CONTROL,
		//{0x20 | 0xC0 | 0x08},
		{0x08},
		1,
		5
	},
	/*{*/
		/*FRAME_RATE_CONTROL_1,*/
		/*{0x00, 0x1A},*/
		/*2,*/
		/*5*/
	/*},*/
	/*{*/
		/*DISPLAY_FUNCTION_CONTROL,*/
		/*{0x08, 0x82, 0x27},*/
		/*3,*/
		/*5*/
	/*},*/
	/*{*/
		/*0xF2,*/
		/*{0x00},*/
		/*1,*/
		/*5*/
	/*},*/
	/*{*/
		/*0x26,*/
		/*{0x01},*/
		/*1,*/
		/*5*/
	/*},*/
	/*{*/
		/*0xE0,*/
		/*{0x0F, 0x2A, 0x2B, 0x08, 0x0E, 0x08, 0x54, 0xA9, 0x43, 0x0A, 0x0F, 0x00, 0x00, 0x00, 0x00},*/
		/*15,*/
		/*5*/
	/*},*/
	/*{*/
		/*0xE1,*/
		/*{0x00, 0x15, 0x17, 0x07, 0x11, 0x06, 0x2B, 0x56, 0x3C, 0x05, 0x10, 0x0F, 0x3F, 0x3F, 0x0F},*/
		/*15,*/
		/*5*/
	/*},*/
	/*{*/
		/*0x2B,*/
		/*{0x00, 0x00, 0x01, 0x3F},*/
		/*4,*/
		/*5*/
	/*},*/
	/*{*/
		/*0x2A,*/
		/*{0x00, 0x00, 0x00, 0xEF},*/
		/*4,*/
		/*5*/
	/*},*/
	{
		SLEEP_OUT,
		{},
		0,
		150
	},
	{
		DISPLAY_ON,
		{},
		0,
		150
	},
};

spi_device_handle_t gSpiHandle;


void sendCommand(CommandCode code)
{
	spi_transaction_t transaction = {};

	transaction.length = BYTES_TO_BITS(1);
	transaction.tx_data[0] = (uint8_t)code;
	transaction.flags = SPI_TRANS_USE_TXDATA;

	gpio_set_level(LCD_PIN_DC, 0);
	spi_device_transmit(gSpiHandle, &transaction);
}


void sendCommandParameters(uint8_t* data, int length)
{
	spi_transaction_t transaction = {};

	transaction.length = BYTES_TO_BITS(length);
	transaction.tx_buffer = data;
	transaction.flags = 0;

	gpio_set_level(LCD_PIN_DC, 1);
	spi_device_transmit(gSpiHandle, &transaction);
}


void frameDraw(uint8_t* buffer)
{
	uint8_t drawWidth[] = { 0,0, UPPER_BYTE_16(LCD_WIDTH), LOWER_BYTE_16(LCD_WIDTH) };
	sendCommand(COLUMN_ADDRESS_SET);
	sendCommandParameters(drawWidth, ARRAY_COUNT(drawWidth));

	uint8_t drawHeight[] = { 0,0, UPPER_BYTE_16(LCD_HEIGHT), LOWER_BYTE_16(LCD_HEIGHT) };
	sendCommand(PAGE_ADDRESS_SET);
	sendCommandParameters(drawHeight, ARRAY_COUNT(drawHeight));

	sendCommand(MEMORY_WRITE);
	sendCommandParameters(buffer, LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH);
}


void setupDisplay()
{
	ESP_LOGI(TASKNAME, "Initializing display...");

	{
		spi_bus_config_t spiBusConfig = {};

		spiBusConfig.miso_io_num = LCD_PIN_MISO;
		spiBusConfig.mosi_io_num = LCD_PIN_MOSI;
		spiBusConfig.sclk_io_num = LCD_PIN_SCLK;
		spiBusConfig.quadwp_io_num = -1;
		spiBusConfig.quadhd_io_num = -1;
		spiBusConfig.max_transfer_sz = LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH;

		ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &spiBusConfig, 1));
		
		ESP_LOGI(TASKNAME, "Initialized spi bus");
	}

	{
		spi_device_interface_config_t spiDeviceConfig = {};

		spiDeviceConfig.clock_speed_hz = SPI_MASTER_FREQ_40M;
		spiDeviceConfig.spics_io_num = LCD_PIN_CS;
		spiDeviceConfig.queue_size = 1;
		spiDeviceConfig.flags = SPI_DEVICE_NO_DUMMY;

		ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &spiDeviceConfig, &gSpiHandle));

		ESP_LOGI(TASKNAME, "Added screen to the spi bus");
	}

	gpio_set_direction(LCD_PIN_DC, GPIO_MODE_OUTPUT);

	int commandCount = ARRAY_COUNT(StartupCommands);

	for (int i = 0; i < commandCount; i++) {
		Command* command = &StartupCommands[i];

		sendCommand(command->code);

		if (command->length > 0) 
			sendCommandParameters(command->parameters, command->length);

		if (command->delay > 0)
			vTaskDelay(command->delay / portTICK_PERIOD_MS);
	}
	ESP_LOGI(TASKNAME, "Initialized display");
}


void app_main(void)
{
	// log starting message
//	char *taskName = pcTaskGetName(NULL);
	ESP_LOGI(TASKNAME, "Hello, starting up");

	setupDisplay();

	uint16_t color = 0xFFFF;

	int x = 50;
	int y = 50;

	int width  = 50;
	int height = 50;

	while (true) {
		memset(frameBuffer, 0, LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH);

		for (int row = y; row < y + height; row++)
			for (int col = x; col < x + width; col++)
				frameBuffer[LCD_WIDTH * row + col] = color;

		frameDraw(frameBuffer);
	}

	esp_restart();
}
