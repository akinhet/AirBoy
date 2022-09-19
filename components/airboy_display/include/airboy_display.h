#pragma once

#include <stdint.h>
#include <stddef.h>
#include "airboy_display_framebuffer.h"

#define DISPLAY_BUS_TYPE 1
#define LCD_PIXEL_CLOCK_HZ  (60 * 1000 * 1000)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create LCD panel for model ILI9341
 *
 * @param[in] buffer_lenght buffer lenght in bytes 
 * @param[in] buffer_alloc  buffer alloc flags (MALLOC_CAP_DMA)
 * @param[in] buffer_count  number of buffers (not working at the moment)    
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
void set_pixel_absolute(uint16_t x, uint16_t y, uint16_t color);

void clear_buffer(uint16_t color);

#ifdef __cplusplus
}
#endif

