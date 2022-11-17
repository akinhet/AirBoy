#pragma once

#include <stdint.h>
#include <stddef.h>
#include "airboy_viewport.h"
#include "airboy_display.h"
#include "airboy_shapes.h"
#include "airboy_math.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draws rectangle with viewport
 * 
 * @param[in] r         rectiangle object
 * @param[in] viewport  viewport object
 */
void draw_rect_viewport(Rectangle *r, viewport_t* viewport);


#ifdef __cplusplus
}
#endif