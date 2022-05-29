#pragma once
#include <stdint.h>

typedef struct {
    int x, y;
    int width, height;
    uint16_t *image;
    int collisionmask;
    int isOnFloor;
    int isOnCeiling;
    int isOnWall;
    uint8_t vFlip;
    uint8_t hFlip;
} Sprite;

typedef struct {
    int width, height;       // max width * height == 256
    uint16_t colormap[256];
    unsigned char tilemap[256][256];
    int collisionmask;
} Tileset;

typedef struct {
    int x, y;
    int index;
    Tileset *parent;
} Tile;


void moveSprite(Sprite *s, int dx, int dy, Sprite *sprites, int ns, Tile *tiles, int nt);
int checkCollision(Sprite *s1, Sprite *s2);
void drawSprite(Sprite s, uint16_t *buffer);

void drawTiles(Tile *tiles, int n, uint16_t *buffer);
