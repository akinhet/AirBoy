#include "assert.h"
#include "airboy_display_primitives.h"
#include "airboy_display.h"

static void plot_line_low(int x0, int y0, int x1, int y1, uint16_t color)
{
	int dx, dy, yi, D, y;

	dx = x1 - x0;
	dy = y1 - y0;
	yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	D = (2 * dy) - dx;
	y = y0;

	for (int x = x0; x <= x1; x++) {
		set_pixel_absolute(x, y, color);
		if ( D > 0 ) {
			y = y + yi;
			D = D + (2 * (dy - dx));
		} else {
			D = D + 2 * dy;
		}
	}
}


static void plot_line_high(int x0, int y0, int x1, int y1, uint16_t color)
{
	int dx, dy, xi, D, x;

	dx = x1 - x0;
	dy = y1 - y0;
	xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}

	D = (2 * dx) - dy;
	x = x0;

	for (int y = y0; y <= y1; y++) {
		set_pixel_absolute(x, y, color);
		if ( D > 0 ) {
			x = x + xi;
			D = D + (2 * (dx - dy));
		} else {
			D = D + 2 * dx;
		}
	}
}

void draw_line(Line l)
{
	if (abs(l.y1 - l.y0) < abs(l.x1 - l.x0))
		if (l.x0 > l.x1)
			plot_line_low(l.x1, l.y1, l.x0 , l.y0, l.color);
	    else
			plot_line_low(l.x0, l.y0, l.x1, l.y1, l.color);
	else
		if (l.y0 > l.y1)
			plot_line_high(l.x1, l.y1, l.x0, l.y0, l.color);
	    else
			plot_line_high(l.x0, l.y0, l.x1, l.y1, l.color);
}

void draw_rect(Rectangle r)
{
	for (int row = r.y; row < r.y + r.height; row++)
		for (int col = r.x; col < r.x + r.width; col++)
			set_pixel_absolute(col, row, r.color);
}