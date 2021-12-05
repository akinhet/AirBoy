#pragma once

#define BYTES_TO_BITS(value)		( (value) * 8 )
#define ARRAY_COUNT(value)			( sizeof(value) / sizeof(value[0]) )
#define UPPER_BYTE_16(value)		( (value) >> 8u )
#define LOWER_BYTE_16(value)		( (value) & 0xFFu )
#define RGB565(red, green, blue)	( (((red) >> 3u) << 11u) | (((green) >> 2u) << 5u) | ((blue) >> 3u))
#define SWAP_ENDIAN_16(value)		( (((value) & 0xFFu) << 8u) | ((value) >> 8u) )
