#include <string.h>
#include <esp_log.h>
#include <driver/i2c.h>

#include "input.h"

#define TAG "Input"

#define INPUT_I2C_ADDR_0	0x27
#define INPUT_I2C_ADDR_1	0x26

#define INPUT_I2C_PORT		0

#define INPUT_REG_IODIR		0x00
#define INPUT_REG_IPOL		0x01
#define INPUT_REG_IOCON		0x05
#define INPUT_REG_GPPU		0x06
#define INPUT_REG_GPIO		0x09


enum buttons {
	// First expander
	DPAD_UP			= 0b00000010,
	DPAD_DOWN		= 0b00010000,
	DPAD_LEFT		= 0b00000100,
	DPAD_RIGHT		= 0b00001000,
	SELECT			= 0b00100000,
	MENU			= 0b01000000,
	START			= 0b10000000,
	BUMPER_LEFT		= 0b00000001,
	// Second expander
	BUMPER_RIGHT	= 0b00000001,
	ACTION_A		= 0b01000000,
	ACTION_B		= 0b10000000,
	ACTION_Y		= 0b00010000,
	ACTION_X		= 0b00100000,
};

void setupInput()
{
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 32,
		.scl_io_num = 33,
		.sda_pullup_en = 0,
		.scl_pullup_en = 0,
		.master.clk_speed = INPUT_CLK_SPEED,
	};

	esp_err_t err;

	// installing i2c driver
	err = i2c_param_config(INPUT_I2C_PORT, &conf);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "I2C PARAM CONFIG FAILED");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "I2C PARAM CONFIG DONE");
#endif

	err = i2c_driver_install(INPUT_I2C_PORT, conf.mode, 0, 0, 0);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "I2C DRIVER INSTALL FAILED");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "I2C DRIVER INSTALL DONE");
#endif

	uint8_t writeBuffer[2];

	// setting up first expander
	writeBuffer[0] = INPUT_REG_IOCON;
	writeBuffer[1] = 0x20;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_0, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_ICON ON EXPANDER 0");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_IOCON ON EXPANDER 0");
#endif

	writeBuffer[0] = INPUT_REG_IODIR;
	writeBuffer[1] = 0xFF;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_0, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_IODIR ON EXPANDER 0");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_IODIR ON EXPANDER 0");
#endif

	writeBuffer[0] = INPUT_REG_IPOL;
	writeBuffer[1] = 0xFF;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_0, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_IPOL ON EXPANDER 0");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_IPOL ON EXPANDER 0");
#endif

	writeBuffer[0] = INPUT_REG_GPPU;
	writeBuffer[1] = 0xFF;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_0, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_GPPU ON EXPANDER 0");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_GPPU ON EXPANDER 0");
#endif


	// setting up second expander
	writeBuffer[0] = INPUT_REG_IOCON;
	writeBuffer[1] = 0x20;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_1, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_IOCON ON EXPANDER 1");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_IOCON ON EXPANDER 1");
#endif

	writeBuffer[0] = INPUT_REG_IODIR;
	writeBuffer[1] = 0xFF;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_1, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_IODIR ON EXPANDER 1");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_IODIR ON EXPANDER 1");
#endif

	writeBuffer[0] = INPUT_REG_IPOL;
	writeBuffer[1] = 0xFF;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_1, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_IPOL ON EXPANDER 1");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_IPOL ON EXPANDER 1");
#endif

	writeBuffer[0] = INPUT_REG_GPPU;
	writeBuffer[1] = 0xFF;
	err = i2c_master_write_to_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_1, writeBuffer, 2, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_GPPU ON EXPANDER 1");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_GPPU ON EXPANDER 1");
#endif
}

Input pollInput()
{
	Input input;

	memset(&input, 0, sizeof(Input));

	const uint8_t reg = INPUT_REG_GPIO;
	uint8_t pins[2];
	esp_err_t err;

	err = i2c_master_write_read_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_0, &reg, 1, &pins, 1, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_GPPU ON EXPANDER 0");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return input;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON READING REG_GPPU ON EXPANDER 0");
#endif
	i2c_master_write_read_device(INPUT_I2C_PORT, INPUT_I2C_ADDR_0, &reg, 1, &pins + 1, 1, 1000/portTICK_RATE_MS);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "FAILURE TO WRITE TO REG_GPPU ON EXPANDER 1");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return input;
	}
#ifdef INPUT_LOGGING
	else
		ESP_LOGI(TAG, "SUCCESS ON READING REG_GPPU ON EXPANDER 1");
#endif

	if (pins[0] & DPAD_UP)
		input.dpad_up = 1;
	if (pins[0] & DPAD_DOWN)
		input.dpad_down = 1;
	if (pins[0] & DPAD_LEFT)
		input.dpad_left = 1;
	if (pins[0] & DPAD_RIGHT)
		input.dpad_right = 1;
	if (pins[0] & BUMPER_LEFT)
		input.bumper_left = 1;
	if (pins[0] & START)
		input.start = 1;
	if (pins[0] & SELECT)
		input.select = 1;
	if (pins[0] & MENU)
		input.menu = 1;
	if (pins[1] & BUMPER_RIGHT)
		input.bumper_right = 1;
	if (pins[1] & ACTION_A)
		input.a = 1;
	if (pins[1] & ACTION_B)
		input.b = 1;
	if (pins[1] & ACTION_X)
		input.x = 1;
	if (pins[1] & ACTION_Y)
		input.y = 1;
	
	return input;
}
