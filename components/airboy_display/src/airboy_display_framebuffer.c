#include <stdint.h>
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "airboy_display_framebuffer.h"

static const char *TAG = "airboy_display_frame_buffer";

esp_err_t init_frame_buffer(const frame_buffer_config_t *config, frame_buffer_t *ret)
{
    esp_err_t err_ret = ESP_OK;
    ESP_GOTO_ON_FALSE(config && ret, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");

    ret->height = config->height;
    ret->width  = config->width;
    ret->buffer_count   = config->buffer_count;
    ret->current_buffer = 0;

    ret->buffer = heap_caps_malloc(sizeof(uint16_t*) * config->buffer_count, config->alloc_flags);
    for (size_t i = 0; i < config->buffer_count; i++)
        ret->buffer[i] = heap_caps_malloc(config->width * config->height * sizeof(uint16_t), config->alloc_flags);

    for (size_t i = 0; i < config->buffer_count; i++)
        ESP_GOTO_ON_FALSE(ret->buffer[i], ESP_ERR_NO_MEM, err, TAG, "no mem for frame buffer %d", i);

    return ESP_OK;
err:
    return err_ret;
}