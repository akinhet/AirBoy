#pragma once

#include <stdint.h>
#include <stddef.h>
#include "airboy_display_framebuffer.h"
#include "airboy_viewport.h"

#define DISPLAY_BUS_TYPE 1
#define LCD_PIXEL_CLOCK_HZ  (40 * 1000 * 1000)

#define SWAP_BYTES(value) ( (((value) & 0xFFu) << 8u) | ((value) >> 8u) )

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create LCD panel for lcd ILI9341
 *
 * @param[in] config pointer to frame buffer config structure
 */
void init_display(frame_buffer_config_t *config);

/**
 * @brief Draw frame buffer to lcd   
 */
void draw_frame();

/**
 * @brief Set pixel on frame buffer
 * 
 * @param[in] x      x pixel coordinate
 * @param[in] y      y pixel coordinate
 * @param[in] color  pixel color
 */
void set_pixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Clears frame buffer with color
 * 
 * @param[in] color  fill color
 */
void clear_buffer(uint16_t color);

void set_pixel_location(int32_t x, int32_t y, uint16_t color, viewport_t* viewport);

#ifdef __cplusplus
}
#endif

