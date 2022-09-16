#pragma once

#include <stdint.h>
#include <stddef.h>

#define DISPLAY_BUS_TYPE 0
#define LCD_PIXEL_CLOCK_HZ  (60 * 1000 * 1000)
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_DEPTH 2
#define LCD_BUFF_SIZE LCD_WIDTH * LCD_HEIGHT * LCD_DEPTH

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
void init_display(size_t buffer_lenght, uint32_t buffer_alloc, uint8_t buffer_count);

#ifdef __cplusplus
}
#endif

