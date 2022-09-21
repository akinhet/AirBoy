#include "airboy_viewport.h"

static const char *TAG = "airboy_viewport";

esp_err_t init_viewport(const viewport_config_t *config, viewport_t *ret)
{
    esp_err_t err_ret = ESP_OK;
    ESP_GOTO_ON_FALSE(config && ret, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");

    ret->x = config->x_start;
    ret->y = config->y_start;
    ret->w = config->width;
    ret->h = config->height;

    return ESP_OK;
err:
    return err_ret;
}
