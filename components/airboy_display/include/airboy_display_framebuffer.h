#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "esp_check.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct frame_buffer_t {
    uint16_t **buffer;
    uint16_t width;
    uint16_t height;
    uint8_t buffer_count;
    uint8_t current_buffer;
}frame_buffer_t;

typedef struct frame_buffer_config_t {
    uint8_t buffer_count;
    uint16_t width;
    uint16_t height;
    uint32_t alloc_flags;
}frame_buffer_config_t;

esp_err_t init_frame_buffer(const frame_buffer_config_t *config, frame_buffer_t *ret);

#ifdef __cplusplus
}
#endif