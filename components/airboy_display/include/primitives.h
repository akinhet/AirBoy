#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x,
        y,
        width,
        height;
    uint16_t color;
} Rectangle;

typedef struct {
    int x0,
        y0,
        x1,
        y1;
    uint16_t color;
} Line;

/**
 * @brief Draws line on framebuffer
 *
 * @param[in] l Line object
 */
void draw_line(Line l);

/**
 * @brief Draws line on framebuffer
 *
 * @param[in] r Rectangle object
 */
void draw_rect(Rectangle r);

#ifdef __cplusplus
}
#endif

