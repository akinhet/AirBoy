#include "sprite.h"
#include <freertos/FreeRTOS.h>

void moveSprite(Sprite *s, int dx, int dy, Sprite *sprites, int ns, Tile *tiles, int nt)
{
    int mask = s->collisionmask,
        newx = s->x + dx,
        newy = s->y + dy;
    for (int i = 0; i < ns; i++) {
        if ( sprites[i].collisionmask == mask )
            if ( newx + s->width >= sprites[i].x && sprites[i].x + sprites[i].width >= newx)
				if ( newy + s->height >= sprites[i].y && sprites[i].y + sprites[i].height >= newy ) {
					newy = s->y < sprites[i].y ? sprites[i].y - s->height : sprites[i].y + sprites[i].height;
					newx = s->x < sprites[i].x ? sprites[i].x - s->width : sprites[i].x + sprites[i].width; 
				} // TODO: rework the collisions
	}
    for (int i = 0; i < nt; i++) {
        if ( tiles[i].parent->collisionmask == mask )
            if ( newx + s->width >= tiles[i].x && tiles[i].x + tiles[i].parent->width >= newx)
				if ( newy + s->height >= tiles[i].y && tiles[i].y + tiles[i].parent->height >= newy ) {
					newy = s->y < tiles[i].y ? tiles[i].y - s->height : tiles[i].y + tiles[i].parent->height;	
					newx = s->x < tiles[i].x ? tiles[i].x - s->width : tiles[i].x + tiles[i].parent->width;
				}
	}

	s->x = newx;
	s->y = newy;
}


void drawSprite(Sprite s, uint16_t *buffer)
{
    for (int row = s.y; row < s.y + s.height; row++)
        for (int col = s.x; col < s.x + s.width; col++)
            buffer[ 320 /* screen height */ * row + col ] = s.image[ s.height * (row - s.y) + (col - s.x) ];
}


void drawTiles(Tile *tiles, int n, uint16_t *buffer)
{
    for ( int i = 0; i < n; i++ )
        for (int row = tiles[i].y; row < tiles[i].y; row++)
            for (int col = tiles[i].x; col < tiles[i].x; col++)
                buffer[ 320 /* screen height */ * row + col ] = tiles[i].parent->colormap[ tiles[i].parent->tilemap[ tiles[i].index ]
                                                                                                                   [ tiles[i].parent->height * (row - tiles[i].y) + (col - tiles[i].x) ]];
}
