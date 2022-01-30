#include "sprite.h"
#include <freertos/FreeRTOS.h>

void moveSprite(Sprite *s, int dx, int dy, Sprite *sprites, int ns, Tile *tiles, int nt)
{
    int mask = s->collisionmask,
        newx = s->x + dx,
        newy = s->y + dy;
    s->isOnFloor = 0;
    s->isOnCeiling = 0;
    s->isOnWall = 0;
    for (int i = 0; i < ns; i++) {
        if ( sprites[i].collisionmask == mask ) {

            if ( newx + s->width >= sprites[i].x && sprites[i].x + sprites[i].width >= newx)
				if ( newy + s->height >= sprites[i].y && sprites[i].y + sprites[i].height >= newy ) {
					/* newy = s->y < sprites[i].y ? sprites[i].y - s->height : sprites[i].y + sprites[i].height; */
					/* newx = s->x < sprites[i].x ? sprites[i].x - s->width : sprites[i].x + sprites[i].width;  */
            /* if (checkCollision(s, &sprites[i])) */
                /* continue; */
                    if (newy + s->height >= sprites[i].y && s->y + s->height < sprites[i].y) {
                        newy = sprites[i].y - s->height - 1;
                        s->isOnFloor = 1;
                    }
                    else if (newy <= sprites[i].y + sprites[i].height && s->y > sprites[i].y + sprites[i].height) {
                        newy = sprites[i].y + sprites[i].height + 1;
                        s->isOnCeiling = 1;
                    }
                    else if (newx + s->width >= sprites[i].x && s->x + s->width < sprites[i].x) {
                        newx = sprites[i].x - s->width - 1;
                        s->isOnWall = 1;
                    }
                    else if (newx <= sprites[i].x + sprites[i].width && s->x > sprites[i].x + sprites[i].width) {
                        newx = sprites[i].x + sprites[i].width + 1;
                        s->isOnWall = 1;
                    }
                }
        }

	}
    for (int i = 0; i < nt; i++) {
        if ( tiles[i].parent->collisionmask == mask )
            if ( newx + s->width >= tiles[i].x && tiles[i].x + tiles[i].parent->width >= newx)
				if ( newy + s->height >= tiles[i].y && tiles[i].y + tiles[i].parent->height >= newy ) {
                    /*
					newy = s->y < tiles[i].y ? tiles[i].y - s->height : tiles[i].y + tiles[i].parent->height;
					newx = s->x < tiles[i].x ? tiles[i].x - s->width : tiles[i].x + tiles[i].parent->width;
					*/
                    if (newy + s->height >= tiles[i].y && s->y + s->height < tiles[i].y) {
                        newy = tiles[i].y - s->height - 1;
                        s->isOnFloor = 1;
                    }
                    else if (newy <= tiles[i].y + tiles[i].parent->height && s->y > tiles[i].y + tiles[i].parent->height) {
                        newy = tiles[i].y + tiles[i].parent->height + 1;
                        s->isOnCeiling = 1;
                    }
                    else if (newx + s->width >= tiles[i].x && s->x + s->width < tiles[i].x) {
                        newx = tiles[i].x - s->width - 1;
                        s->isOnWall = 1;
                    }
                    else if (newx <= tiles[i].x + tiles[i].parent->width && s->x > tiles[i].x + tiles[i].parent->width) {
                        newx = tiles[i].x + tiles[i].parent->width + 1;
                        s->isOnWall = 1;
                    }
				}
	}

	s->x = newx;
	s->y = newy;
}


int checkCollision(Sprite *s1, Sprite *s2)
{
    return (s1->x + s1->width >= s2->x && s2->x + s2->width >= s1->x && s1->y + s1->height >= s2->y && s2->y + s2->height >= s1->y) ? 1 : 0;
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
