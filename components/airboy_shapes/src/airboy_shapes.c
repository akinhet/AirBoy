#include "assert.h"
#include "airboy_shapes.h"
#include "airboy_display.h"

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

/*static void plot_line_low(int x0, int y0, int x1, int y1, uint16_t color)
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
}*/

/*static void draw_circle_section(int x, int y, int x0, int y0, uint16_t color)
{
    set_pixel_absolute(x0 + x, y0 - y, color);
    set_pixel_absolute(x0 + y, y0 - x, color);
    set_pixel_absolute(x0 - x, y0 - y, color);
    set_pixel_absolute(x0 - y, y0 - x, color);
    set_pixel_absolute(x0 + x, y0 + y, color);
    set_pixel_absolute(x0 + y, y0 + x, color);
    set_pixel_absolute(x0 - x, y0 + y, color);
    set_pixel_absolute(x0 - y, y0 + x, color);
}*/

void draw_rect(Rectangle *r, viewport_t* viewport)
{
	int x, y, h, w;
	x = r->x;
	y = r->y;
	h = r->h;
	w = r->w;

	if (viewport) 
	{
		x = r->x - viewport->x;
		y = r->y - viewport->y;

		if (x + w < 0 || y + h < 0) return;
		if (x - w > viewport->w || y - h > viewport->h) return;

		/*x = max(r->x, viewport->x);
		w = min((r->w + r->x), (viewport->w + viewport->x));
		w -= x;
		if (w < 0) return;

		h = min((r->h + r->y), (viewport->h + viewport->y));
		y = max(r->y, viewport->y);
		h -= y;
		if (h < 0) return;

		x = r->x - viewport->x;
		y = r->y - viewport->y;*/
	}

	for (int row = y; row < y + h; row++)
		for (int col = x; col < x + w; col++)
			set_pixel(col, row, r->color);
}

/*void draw_circle(circle c)
{
  int f, ddF_x, ddF_y;
  int x, y;

  f = 1;
  f -= c.rad;
  ddF_x = 1;
  ddF_y = 0;
  ddF_y -= c.rad;
  ddF_y *= 2;
  x = 0;
  y = c.rad;

  draw_circle_section(x, y, c.x, c.y, c.color);
  
  while ( x < y )
  {
    if (f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    draw_circle_section(x, y, c.x, c.y, c.color);    
  }
}*/