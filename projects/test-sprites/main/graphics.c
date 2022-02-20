#include "lib/macros.h"
#include "graphics.h"

const uint16_t bricks_r = SWAP_ENDIAN_16(RGB565(0xFF, 0, 0));
const uint16_t bricks_w = 0xFFFE;

uint16_t bricks_sprite[256] = {
	bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
	bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
	bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
	bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
	bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
	bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
	bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
	bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,bricks_w,bricks_r,bricks_r,bricks_r,
	bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,bricks_w,
};

const uint16_t coin_t = 0xFFFF; //transparent
const uint16_t coin_b = 0;
const uint16_t coin_y = SWAP_ENDIAN_16(RGB565(0xFF, 0xDF, 0x76));
const uint16_t coin_g = SWAP_ENDIAN_16(RGB565(0xFB, 0xC2, 0x3F));
const uint16_t coin_o = SWAP_ENDIAN_16(RGB565(0xEB, 0x93, 0x3A));

uint16_t coin_sprite[64] = {
	coin_t,coin_t,coin_b,coin_b,coin_b,coin_b,coin_t,coin_t,
	coin_t,coin_b,coin_g,coin_y,coin_y,coin_y,coin_b,coin_t,
	coin_b,coin_g,coin_g,coin_o,coin_o,coin_g,coin_y,coin_b,
	coin_b,coin_g,coin_o,coin_g,coin_g,coin_y,coin_g,coin_b,
	coin_b,coin_g,coin_o,coin_g,coin_g,coin_y,coin_g,coin_b,
	coin_b,coin_o,coin_g,coin_y,coin_y,coin_g,coin_g,coin_b,
	coin_t,coin_b,coin_o,coin_o,coin_o,coin_g,coin_b,coin_t,
	coin_t,coin_t,coin_b,coin_b,coin_b,coin_b,coin_t,coin_t,
};

const uint16_t player_t = 0xFFFF;
const uint16_t player_a = SWAP_ENDIAN_16(RGB565(0x71, 0x5A, 0x42)); //1
const uint16_t player_b = SWAP_ENDIAN_16(RGB565(0x92, 0x73, 0x53)); //2
const uint16_t player_c = SWAP_ENDIAN_16(RGB565(0xFA, 0xCF, 0xA5)); //3
const uint16_t player_d = SWAP_ENDIAN_16(RGB565(0xEB, 0xBB, 0x8A)); //4
const uint16_t player_e = SWAP_ENDIAN_16(RGB565(0x1A, 0x9A, 0x5A)); //5
const uint16_t player_f = SWAP_ENDIAN_16(RGB565(0xAE, 0x31, 0x23)); //6
const uint16_t player_g = SWAP_ENDIAN_16(RGB565(0xC6, 0x5B, 0x4F)); //7
const uint16_t player_h = SWAP_ENDIAN_16(RGB565(0x64, 0x74, 0x8E)); //8
const uint16_t player_i = SWAP_ENDIAN_16(RGB565(0x8C, 0xA3, 0xC6)); //9
const uint16_t player_j = SWAP_ENDIAN_16(RGB565(0x50, 0x5B, 0x6D)); //10
const uint16_t player_k = SWAP_ENDIAN_16(RGB565(0xBA, 0x4C, 0x0A)); //11
const uint16_t player_l = SWAP_ENDIAN_16(RGB565(0x92, 0x43, 0x13)); //12

/*
uint16_t player_sprite[98] = {
	player_t,player_a,player_b,player_a,player_b,player_b,player_t,
	player_t,player_a,player_c,player_c,player_c,player_c,player_t,
	player_t,player_d,player_c,player_e,player_c,player_e,player_t,
	player_t,player_d,player_c,player_c,player_c,player_c,player_t,
	player_t,player_t,player_d,player_c,player_c,player_t,player_t,
	player_t,player_f,player_g,player_g,player_g,player_g,player_t,
	player_f,player_f,player_g,player_g,player_g,player_g,player_t,
	player_f,player_f,player_g,player_g,player_g,player_g,player_c,
	player_d,player_d,player_g,player_g,player_g,player_g,player_c,
	player_t,player_d,player_h,player_h,player_i,player_i,player_t,
	player_t,player_j,player_h,player_t,player_i,player_i,player_t,
	player_t,player_h,player_h,player_t,player_i,player_t,player_t,
	player_t,player_j,player_t,player_t,player_k,player_k,player_t,
	player_t,player_l,player_l,player_t,player_t,player_t,player_t,
};
*/

uint16_t player_sprite[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
