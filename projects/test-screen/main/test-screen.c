#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_DEPTH 2

#define BYTES_TO_BITS(value) ((value) * 8)

enum pins{
	LCD_PIN_MISO	= 19,
	LCD_PIN_MOSI	= 23,
	LCD_PIN_SCLK	= 18,
	LCD_PIN_CS		= 15,
	LCD_PIN_DC		=  2,
	LCD_PIN_RESET	=  4,
};

typedef enum{
	SOFTWARE_RESET			= 0x01u,
	DISPLAY_OFF				= 0x28u,
	DISPLAY_ON				= 0x29u,
	ENTER_SLEEP_MODE		= 0x10u,
	SLEEP_OUT				= 0x11u,
	DISPLAY_INVERSION_OFF	= 0x20u,
	DISPLAY_INVERSION_ON	= 0x21u,
	COLUMN_ADDRESS_SET		= 0x2Au,
	PAGE_ADDRESS_SET		= 0x2Bu,
	MEMORY_WRITE			= 0x2Cu,
	MEMORY_ACCESS_CONTROL	= 0x36u,
	PIXEL_FORMAT_SET		= 0x3Au,
} CommandCode;

typedef struct{
	CommandCode code;
	uint8_t parameters[15];
	uint8_t length;
} Command;

Command StartupCommands[] = {
	{ 
		SOFTWARE_RESET, 
		{}, 
		0 
	},
	{
		MEMORY_ACCESS_CONTROL,
		{0x20 | 0xc0 | 0x08},
		1
	},
	{
		PIXEL_FORMAT_SET,
		{0x55},
		1
	},
	{
		SLEEP_OUT,
		{},
		0
	},
	{
		DISPLAY_ON,
		{},
		0
	},
};

spi_bus_config_t spiBusConfig = {};

spiBusConfig.miso_io_num = LCD_PIN_MISO;
spiBusConfig.mosi_io_num = LCD_PIN_MOSI;
spiBusConfig.sclk_io_num = LCD_PIN_SCLK;
spiBusConfig.quadwp_io_num = -1;
spiBusConfig.quadhp_io_num = -1;
spiBusConfig.max_transfer_sz = LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH;

spi_handle_t gSpiHandle;

spi_device_interface_config_t spiDeviceConfig = {};

spiDeviceConfig.clock_speed_hz = SPI_MASTER_FREQ_40M;
spiDeviceConfig.spics_io_num = LCD_PIN_CS;
spiDeviceConfig.queue_size = 1;
spiDeviceConfig.flags = SPI_DEVICE_NO_DUMMY;


void sendCommand(CommandCode code)
{
	spi_transaction_t transaction = {};

	transaction.length = BYTES_TO_BITS(1);
	transaction.tx_data[0] = (uint8_t)code;
	transaction.flags = SPI_TRANS_USE_TXDATA;

	gpio_set_level(LCD_PIN_DC, 0);
	spi_device_transmit(gSpiHandle, &transaction);
}


void app_main(void)
{
	// log starting message
	char *taskName = pcTaskGetName(NULL);
	ESP_LOGI(taskName, "Hello, starting up\n");

	ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &spiBusConfig, 1));
	ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &spiDeviceConfig, &gSpiHandle));

	gpio_set_direction(LCD_PIN_DC, GPIO_MODE_OUTPUT);
}
