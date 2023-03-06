#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The buffer for the input buttons
 */
typedef struct {
	struct {
		uint16_t dpad_up		: 1;
		uint16_t dpad_down		: 1;
		uint16_t dpad_left		: 1;
		uint16_t dpad_right		: 1;
		uint16_t select			: 1;
		uint16_t menu			: 1;
		uint16_t start			: 1;
		uint16_t bumper_left	: 1;
		uint16_t bumper_right	: 1;
		uint16_t a				: 1;
		uint16_t b				: 1;
		uint16_t x				: 1;
		uint16_t y				: 1;
	} prev, curr;
} input_buffer_t;

/**
 * @brief Initialize input_buffer_t
 * @param[in] input_buffer pointer to the input_buffer_t to be initialized
 */
void init_input(input_buffer_t *input_buffer);

/**
 * @brief Update input_buffer_t
 * @param[in] input_buffer pointer to the input_buffer_t to be updated
 */
void update_input(input_buffer_t *input_buffer);

#ifdef __cplusplus
}
#endif

