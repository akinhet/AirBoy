#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"
#include "esp_check.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Frame buffer structure
 */
typedef struct viewport_t {
    int32_t x,   // Viewport origin x position
            y,   // Viewport origin y position
            w,   // Viewport width
            h;   // Viewport height
}viewport_t;

/**
 * @brief Frame buffer config structure
 */
typedef struct viewport_config_t {
    int32_t x_start,  // Viewport origin x start position
            y_start,  // Viewport origin y start position
            width,    // Viewport width
            height;   // Viewport height
}viewport_config_t;

esp_err_t init_viewport(const viewport_config_t *config, viewport_t *ret);

#ifdef __cplusplus
}
#endif

