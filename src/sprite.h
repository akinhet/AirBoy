#pragma once

typedef struct {
    int x, y;
    int width, height;
    uint16_t *image;
    int collisionmask;
} Sprite;

typedef struct {
    int width, height;       // max width * height == 256
    uint16_t colormap[256];
    char tilemap[256][256];
    int collisionmask;
} Tileset;

typedef struct {
    int x, y;
    int index;
    Tileset *parent;
} Tile;


void moveSprite(Sprite s, int dx, int dy, Sprite *sprites, int ns, Tile *tiles, int nt);
void drawSprite(Sprite s, uint16_t *buffer);

void drawTiles(Tile *tiles, uint16_t *buffer);
