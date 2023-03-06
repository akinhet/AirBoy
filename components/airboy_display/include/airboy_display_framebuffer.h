#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "esp_check.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Frame buffer structure
 */
typedef struct frame_buffer_t {
    uint16_t **buffer;      // Pointer to frame buffer array
    uint16_t width;         // Frame buffer width
    uint16_t height;        // Frame buffer height
    uint8_t buffer_count;   // Number of frame buffers
    uint8_t current_buffer; // Current selected buffer
    bool write_enable;      // 
} frame_buffer_t;

/**
 * @brief Frame buffer config structure
 */
typedef struct frame_buffer_config_t {
    uint8_t buffer_count; // Number of buffers NOTE: if more than one allocate in the spiram
    uint16_t width;       // Screen width
    uint16_t height;      // Screen height
    uint32_t alloc_flags; // Frame buffer alocation flags default: MALLOC_CAP_DMA
} frame_buffer_config_t;


/**
 * @brief Init frame buffer for lcd
 *
 * @param[in] config pointer to frame buffer config structure
 * @param[out] ret returned frame buffer structure
 * @return
 *          - ESP_ERR_INVALID_ARG   if parameter is invalid
 *          - ESP_ERR_NO_MEM        if out of memory
 *          - ESP_OK                on success
 */
esp_err_t init_frame_buffer(const frame_buffer_config_t *config, frame_buffer_t *ret);

#ifdef __cplusplus
}
#endif

