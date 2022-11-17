#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SWAP_BYTES(value) ((((value) & 0xFFu) << 8u) | ((value) >> 8u))

#define max(x, y) (((x) >= (y)) ? (x) : (y))
#define min(x, y) (((x) <= (y)) ? (x) : (y))

typedef struct vector2i {
    int32_t x,
            y;
} vector2i;

typedef struct vector2f {
    float x,
          y;
} vector2f;

#ifdef __cplusplus
}
#endif

