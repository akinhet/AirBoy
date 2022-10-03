#pragma once

#include "airboy_viewport.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x,
        y,
        w,
        h;
    uint16_t color;
} Rectangle;

typedef struct {
    int x0,
        y0,
        x1,
        y1;
    uint16_t color;
} Line;

typedef struct {
    int x,
        y,
        rad;
    uint16_t color;
} circle;

/**
 * @brief Draws line on framebuffer
 *
 * @param[in] l Line object
 */
//void draw_line(Line l);

/**
 * @brief Draws line on framebuffer
 *
 * @param[in] r Rectangle object
 */
void draw_rect(Rectangle *r, viewport_t *viewport);

//void draw_circle(circle c);

#ifdef __cplusplus
}
#endif

