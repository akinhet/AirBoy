#pragma once

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Write to the specified register
 * @param[in] ic_address address of the MCP23008 IC
 * @param[in] reg_address address of the register on the IC
 * @param[in] value value to be written to the specified register
 * @return
 *          - ESP_ERR_INVALID_ARG   parameter is invalid
 *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode
 *			- ESP_ERR_TIMEOUT		operation timeout because the bus is busy
 *			- ESP_FAIL				sending command error, slave hasn’t ACK the transfer
 *          - ESP_OK                on success
 */
esp_err_t mcp23008_write_reg(uint8_t ic_address, uint8_t reg_address, uint8_t value);

/**
 * @brief Read from the specified register
 * @param[in] ic_address address of the MCP23008 IC
 * @param[in] reg_address address of the register on the IC
 * @param[out] value value read from the register
 * @return
 *          - ESP_ERR_INVALID_ARG   parameter is invalid
 *			- ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode
 *			- ESP_ERR_TIMEOUT		operation timeout because the bus is busy
 *			- ESP_FAIL				sending command error, slave hasn’t ACK the transfer
 *          - ESP_OK                on success
 */
esp_err_t mcp23008_read_reg(uint8_t ic_address, uint8_t reg_address, uint8_t* value);

#ifdef __cplusplus
}
#endif

