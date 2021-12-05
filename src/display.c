#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <driver/spi_common.h>

#include "display.h"
#include "macros.h"

#define TASKNAME "display"

enum lcdpins{
	LCD_PIN_MISO	= 19,
	LCD_PIN_MOSI	= 23,
	LCD_PIN_SCLK	= 18,
	LCD_PIN_CS		= 15,
	LCD_PIN_DC		=  2,
	LCD_PIN_RESET	=  4,
};

typedef enum{
	SOFTWARE_RESET				= 0x01,
	DISPLAY_OFF					= 0x28,
	DISPLAY_ON					= 0x29,
	ENTER_SLEEP_MODE			= 0x10,
	SLEEP_OUT					= 0x11,
	DISPLAY_INVERSION_OFF		= 0x20,
	DISPLAY_INVERSION_ON		= 0x21,
	COLUMN_ADDRESS_SET			= 0x2A,
	PAGE_ADDRESS_SET			= 0x2B,
	MEMORY_WRITE				= 0x2C,
	MEMORY_ACCESS_CONTROL		= 0x36,
	PIXEL_FORMAT_SET			= 0x3A,
	FRAME_RATE_CONTROL_1		= 0xB1,
	DISPLAY_FUNCTION_CONTROL	= 0xB6,
	POWER_CONTROL_1				= 0xC0,
	POWER_CONTROL_2				= 0xC1,
	VCOM_CONTROL_1				= 0xC5,
	VCOM_CONTROL_2				= 0xC7,
} CommandCode;

typedef struct{
	CommandCode code;
	uint8_t parameters[15];
	uint8_t length;
	int delay;
} Command;

Command StartupCommands[] = {
	{ 
		SOFTWARE_RESET, 
		{}, 
		0,
		150
	},
	{
		PIXEL_FORMAT_SET,
		{0x55},
		1,
		5
	},
	{
		MEMORY_ACCESS_CONTROL,
		{0x20 | 0xC0 | 0x08},
		//{0x08},
		1,
		5
	},
	/*{
		FRAME_RATE_CONTROL_1,
		{0x00, 0x1A},
		2,
		5
	},*/
	/*{
		DISPLAY_FUNCTION_CONTROL,
		{0x08, 0x82, 0x27},
		3,
		5
	},*/
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

static spi_device_handle_t gSpiHandle;

void sendCommand(CommandCode code)
{
	spi_transaction_t transaction = {};

	transaction.length = BYTES_TO_BITS(1);
	transaction.tx_data[0] = (uint8_t)code;
	transaction.flags = SPI_TRANS_USE_TXDATA;

	gpio_set_level(LCD_PIN_DC, 0);
	ESP_ERROR_CHECK(spi_device_transmit(gSpiHandle, &transaction));
	ESP_LOGI(TASKNAME, "Send command");
}


void sendCommandParameters(uint8_t* data, int length)
{
	spi_transaction_t transaction = {};

	transaction.length = BYTES_TO_BITS(length);
	transaction.tx_buffer = data;
	transaction.flags = 0;

	gpio_set_level(LCD_PIN_DC, 1);
	ESP_ERROR_CHECK(spi_device_transmit(gSpiHandle, &transaction));
	ESP_LOGI(TASKNAME, "Send parameters");
}


void frameDraw(uint16_t* buffer)
{
	uint8_t drawWidth[] = { 0,0, UPPER_BYTE_16(LCD_WIDTH), LOWER_BYTE_16(LCD_WIDTH) };
	sendCommand(COLUMN_ADDRESS_SET);
	sendCommandParameters(drawWidth, ARRAY_COUNT(drawWidth));

	uint8_t drawHeight[] = { 0,0, UPPER_BYTE_16(LCD_HEIGHT), LOWER_BYTE_16(LCD_HEIGHT) };
	sendCommand(PAGE_ADDRESS_SET);
	sendCommandParameters(drawHeight, ARRAY_COUNT(drawHeight));

	sendCommand(MEMORY_WRITE);
	sendCommandParameters((uint8_t*)buffer, LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH);
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
	gpio_set_direction(LCD_PIN_RESET, GPIO_MODE_OUTPUT);

	gpio_set_level(LCD_PIN_RESET, 1);
	vTaskDelay(5 / portTICK_PERIOD_MS);
	gpio_set_level(LCD_PIN_RESET, 0);
	vTaskDelay(20 / portTICK_PERIOD_MS);
	gpio_set_level(LCD_PIN_RESET, 1);



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
