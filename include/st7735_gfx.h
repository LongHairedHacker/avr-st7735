#ifndef _ST7735_GFX_H_
#define _ST7735_GFX_H_

#include<stdint.h>

void st7735_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
void st7735_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,  uint16_t color);
void st7735_draw_circle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color);

#endif
