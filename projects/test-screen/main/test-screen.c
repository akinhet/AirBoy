#include <stdlib.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

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
	INPUT_CLK		= 25,
	INPUT_LATCH		= 33,
	INPUT_IN		= 32,
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

typedef struct{
	uint16_t dpad_up		: 1; //  7
	uint16_t dpad_down		: 1; //  2
	uint16_t dpad_left		: 1; //  5
	uint16_t dpad_right		: 1; //  4
	uint16_t select			: 1; //  3
	uint16_t start			: 1; //  0
	uint16_t bumper_left	: 1; //  6
	uint16_t bumper_right	: 1; // 10
	uint16_t a				: 1; //  9
	uint16_t b				: 1; //  1
	uint16_t x				: 1; // 11
	uint16_t y				: 1; //  8
} Input;

typedef struct{
	int x;
	int y;
} Vec2;

static uint16_t frameBuffer[LCD_WIDTH * LCD_HEIGHT];


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

void setupInput()
{
	// set up pins
	gpio_reset_pin(INPUT_CLK);
	gpio_set_direction(INPUT_CLK, GPIO_MODE_OUTPUT);
	gpio_set_pull_mode(INPUT_CLK, GPIO_PULLUP_ONLY);
	
	gpio_reset_pin(INPUT_LATCH);
	gpio_set_direction(INPUT_LATCH, GPIO_MODE_OUTPUT);
	gpio_set_pull_mode(INPUT_LATCH, GPIO_PULLUP_ONLY);

	gpio_reset_pin(INPUT_IN);
	gpio_set_direction(INPUT_IN, GPIO_MODE_INPUT);
	gpio_set_pull_mode(INPUT_IN, GPIO_PULLUP_ONLY);
}

Input pollInput()
{
	//TODO: fix timings
	// refresh bit shifter
	gpio_set_level(INPUT_LATCH, 0);
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(5 / portTICK_PERIOD_MS);
	gpio_set_level(INPUT_LATCH, 1);
	vTaskDelay(5 / portTICK_PERIOD_MS);

	Input input;

	// get pin states
	input.start = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.b = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.dpad_down = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.select = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.dpad_right = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.dpad_left = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.bumper_left = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.dpad_up = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.y = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.a = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.bumper_right = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	input.x = !gpio_get_level(INPUT_IN);
	gpio_set_level(INPUT_CLK, 0);
	/*vTaskDelay(5 / portTICK_PERIOD_MS);*/
	gpio_set_level(INPUT_CLK, 1);
	vTaskDelay(3 / portTICK_PERIOD_MS);

	return input;
}


void app_main(void)
{
	// log starting message
	ESP_LOGI(TASKNAME, "Hello, starting up");

	setupDisplay();
	setupInput();

	Input input;

	Vec2 velocity = {5, 5};

	uint16_t color = 0xFFFF;
	uint16_t background = 0;

	int x = 50;
	int y = 50;

	int width  = 30;
	int height = 30;

	while (true) {
		input = pollInput();

		if (input.dpad_up)
			velocity.y -= 1;
		if (input.dpad_down)
			velocity.y += 1;
		if (input.dpad_right)
			velocity.x += 1;
		if (input.dpad_left)
			velocity.x -= 1;
		if (input.a)
			color = SWAP_ENDIAN_16(RGB565(0xFF,0,0));
		if (input.b)
			color = SWAP_ENDIAN_16(RGB565(0,0xFF,0));
		if (input.x)
			color = SWAP_ENDIAN_16(RGB565(0,0,0xFF));
		if (input.y)
			color = SWAP_ENDIAN_16(RGB565(0xFF,0xFF,0xFF));
		if (input.start)
			background = SWAP_ENDIAN_16(RGB565(0,0xFF,0));
		if (input.select)
			background = SWAP_ENDIAN_16(RGB565(0,0,0xFF));
		if (input.bumper_left)
			background = SWAP_ENDIAN_16(RGB565(0xFF,0,0));
		if (input.bumper_right)
			background = SWAP_ENDIAN_16(RGB565(0,0,0));


		if (x+width >= LCD_WIDTH || x <= 0)
			velocity.x = -velocity.x;
		if (y+height >= LCD_HEIGHT || y <= 0)
			velocity.y = -velocity.y;

		x += velocity.x;
		y += velocity.y;

		//memset(frameBuffer, background, LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH);
		for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
			frameBuffer[i] = background;

		for (int row = y; row < y + height; row++)
			for (int col = x; col < x + width; col++)
				frameBuffer[LCD_WIDTH * row + col] = color;

		frameDraw(frameBuffer);
	}

	esp_restart();
}
