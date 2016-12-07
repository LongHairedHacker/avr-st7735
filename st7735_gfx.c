#include "st7735_gfx.h"

#include<stdlib.h>

#include "st7735.h"

#define _swap_uint8_t(a, b) { uint8_t t = a; a = b; b = t; }

void st7735_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color) {
	uint8_t steep_dir = abs(y1 - y0) > abs(x1 - x0);
	if (steep_dir) {
		_swap_uint8_t(x0, y0);
		_swap_uint8_t(x1, y1);
	}

	if (x0 > x1) {
		_swap_uint8_t(x0, x1);
		_swap_uint8_t(y0, y1);
	}

  	int16_t dx, dy;
  	dx = x1 - x0;
  	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t y_step;

	if (y0 < y1) {
		y_step = 1;
	} else {
		y_step = -1;
	}

	uint8_t seg = x0;
	uint8_t cur_x;
	for(cur_x = x0; cur_x <= x1; cur_x++) {
		err -= dy;
		if (err < 0) {
			if (steep_dir) {
				st7735_draw_fast_vline(y0, seg, cur_x - seg + 1, color);
			} else {
				st7735_draw_fast_hline(seg, y0, cur_x - seg +1, color);
			}
			y0 += y_step;
			err += dx;
			seg = cur_x + 1;
		}
	}

	// x0 incremented
	if (steep_dir) {
		st7735_draw_fast_vline(y0, seg, cur_x - seg, color);
	} else {
		st7735_draw_fast_hline(seg, y0, cur_x - seg, color);
	}
}


void st7735_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
	if(w < 1 || h < 1) {
			return;
	}

	st7735_draw_fast_hline(x, y, w, color);
	st7735_draw_fast_hline(x, y + h - 1, w, color);
	st7735_draw_fast_vline(x, y, h, color);
	st7735_draw_fast_vline(x + w - 1, y, h, color);
}


void st7735_draw_circle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	st7735_draw_pixel(x0, y0 + r, color);
	st7735_draw_pixel(x0, y0 - r, color);
	st7735_draw_pixel(x0 + r, y0, color);
	st7735_draw_pixel(x0 - r, y0, color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		st7735_draw_pixel(x0 + x, y0 + y, color);
		st7735_draw_pixel(x0 - x, y0 + y, color);
		st7735_draw_pixel(x0 + x, y0 - y, color);
		st7735_draw_pixel(x0 - x, y0 - y, color);
		st7735_draw_pixel(x0 + y, y0 + x, color);
		st7735_draw_pixel(x0 - y, y0 + x, color);
		st7735_draw_pixel(x0 + y, y0 - x, color);
		st7735_draw_pixel(x0 - y, y0 - x, color);
	}
}
