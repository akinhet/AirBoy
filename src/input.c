#include "input.h"

#define INPUT_I2C_PORT      0

// definitions of all expander registers that airboy use at the moment
#define INPUT_REG_IODIR     0x00
#define INPUT_REG_IPOL      0x01
#define INPUT_REG_IOCON     0x05
#define INPUT_REG_GPPU      0x06
#define INPUT_REG_GPIO      0x09

#define ACK_CHECK_EN   0x1 
#define ACK_VAL        0x0
#define NACK_VAL       0x1

#define TAG "Input"

enum buttons {
	// First expander
	DPAD_UP	        = 0b00000010,
	DPAD_DOWN       = 0b00010000,
	DPAD_LEFT       = 0b00000100,
	DPAD_RIGHT      = 0b00001000,
	SELECT          = 0b00100000,
	MENU            = 0b01000000,
	START           = 0b10000000,
	BUMPER_LEFT     = 0b00000001,

	// Second expander
	BUMPER_RIGHT    = 0b00000001,
	ACTION_A        = 0b01000000,
	ACTION_B        = 0b10000000,
	ACTION_Y        = 0b00010000,
	ACTION_X        = 0b00100000,
};

// function returns the value of the read register of the input ic
esp_err_t mcp23008_write_reg(uint8_t ICAddress, uint8_t regAddress, uint8_t value) 
{

#ifdef INPUT_LOGGING
	ESP_LOGI(TAG, "write reg MCP port %d address %d", INPUT_I2C_PORT, regAddress);
#endif

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ICAddress << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, regAddress, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, value, ACK_CHECK_EN);
	i2c_master_stop(cmd);

	esp_err_t ret = i2c_master_cmd_begin(INPUT_I2C_PORT, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	if (ret != ESP_OK) 
	{
		ESP_LOGE(TAG, "Error writing register at address %d", regAddress);
		return ESP_FAIL;
	}

#ifdef INPUT_LOGGING
	else ESP_LOGI(TAG, "SUCCESS ON WRITING TO REG_GPPU ON EXPANDER 1");
#endif

	return ESP_OK;
}

esp_err_t mcp23008_read_reg(uint8_t ICAddress, uint8_t regAddress, uint8_t* value)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ICAddress << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, regAddress, ACK_CHECK_EN);
	i2c_master_stop(cmd);

	esp_err_t ret = i2c_master_cmd_begin(INPUT_I2C_PORT, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	if (ret != ESP_OK) 
	{
		ESP_LOGE(TAG, "Error one reading register %d", regAddress);
		return ESP_FAIL;
	}

#ifdef INPUT_LOGGING
	else ESP_LOGI(TAG, "SUCCESS ON READING REG_GPPU ON EXPANDER ADRESS %d", regAddress);
#endif

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ICAddress << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
	i2c_master_read_byte(cmd, value, ACK_CHECK_EN);
	i2c_master_stop(cmd);

	ret = i2c_master_cmd_begin(INPUT_I2C_PORT, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	if (ret != ESP_OK) 
	{
		ESP_LOGE(TAG, "Error two reading register %d", regAddress);
		return ESP_FAIL;
	}

#ifdef INPUT_LOGGING
	else ESP_LOGI(TAG, "SUCCESS ON READING REG_GPPU ON EXPANDER ADRESS %d", regAddress);
#endif

	return ESP_OK;
}

void setupInput()
{
	// i2c configuration
	i2c_config_t conf =
	{
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 32,
		.sda_pullup_en = GPIO_PULLUP_DISABLE,
		.scl_io_num = 33,
		.scl_pullup_en = GPIO_PULLUP_DISABLE,
		.master.clk_speed = INPUT_CLK_SPEED,
		// .clk_flags = 0,
	};

	// input errors variable
	esp_err_t err;

	// installing i2c driver
	err = i2c_param_config(INPUT_I2C_PORT, &conf);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "I2C PARAM CONFIG FAILED");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}

#ifdef INPUT_LOGGING
	else ESP_LOGI(TAG, "I2C PARAM CONFIG DONE");
#endif

	err = i2c_driver_install(INPUT_I2C_PORT, conf.mode, 0, 0, 0);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "I2C DRIVER INSTALL FAILED");
		ESP_LOGE(TAG, "RECIVED ERROR CODE: %s", esp_err_to_name(err));
		return;
	}

#ifdef INPUT_LOGGING
	else ESP_LOGI(TAG, "I2C DRIVER INSTALL DONE");
#endif

	// setting up first expander
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_0, INPUT_REG_IODIR, 0xFF));
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_0, INPUT_REG_IPOL, 0xFF));
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_0, INPUT_REG_GPPU, 0xFF));
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_0, INPUT_REG_IOCON, 0B00110000));

	// setting up second expander
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_1, INPUT_REG_IODIR, 0xFF));
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_1, INPUT_REG_IPOL, 0xFF));
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_1, INPUT_REG_GPPU, 0xFF));
	ESP_ERROR_CHECK(mcp23008_write_reg(INPUT_I2C_ADDR_1, INPUT_REG_IOCON, 0B00110000));
}

Input pollInput()
{
	uint8_t writeBuffer[2];
	Input input;
	memset(&input, 0, sizeof(Input));

	// read first expander
	mcp23008_read_reg(INPUT_I2C_ADDR_0, INPUT_REG_GPIO, &writeBuffer[0]);

	//read second expander
	mcp23008_read_reg(INPUT_I2C_ADDR_1, INPUT_REG_GPIO, &writeBuffer[1]);

	if (writeBuffer[0] & DPAD_UP)       input.dpad_up = 1;
	if (writeBuffer[0] & DPAD_DOWN)     input.dpad_down = 1;
	if (writeBuffer[0] & DPAD_LEFT)     input.dpad_left = 1;
	if (writeBuffer[0] & DPAD_RIGHT)    input.dpad_right = 1;
	if (writeBuffer[0] & BUMPER_LEFT)   input.bumper_left = 1;
	if (writeBuffer[0] & START)         input.start = 1;
	if (writeBuffer[0] & SELECT)        input.select = 1;
	if (writeBuffer[0] & MENU)          input.menu = 1;
	if (writeBuffer[1] & BUMPER_RIGHT)  input.bumper_right = 1;
	if (writeBuffer[1] & ACTION_A)      input.a = 1;
	if (writeBuffer[1] & ACTION_B)      input.b = 1;
	if (writeBuffer[1] & ACTION_X)      input.x = 1;
	if (writeBuffer[1] & ACTION_Y)      input.y = 1;
	
	return input;
}
