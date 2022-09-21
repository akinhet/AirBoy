#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

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

