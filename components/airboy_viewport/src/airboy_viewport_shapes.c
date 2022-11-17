#include "airboy_viewport_shapes.h"

void draw_rect_viewport(Rectangle *r, viewport_t* viewport)
{
	int x, y, h, w;

	x = max(r->x, viewport->x);
	w = min((r->w + r->x), (viewport->w + viewport->x));
	if (x > w) return;
	w -= x;
		
	y = max(r->y, viewport->y);
	h = min((r->h + r->y), (viewport->h + viewport->y));
	if (y > h) return;
	h -= y;

	x -= viewport->x;
	y -= viewport->y;

	for (int row = y; row < y + h; row++)
		for (int col = x; col < x + w; col++)
			set_pixel(col, row, r->color);
}