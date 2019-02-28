#ifndef _ST7735_H_
#define _ST7735_H_

#include <avr/pgmspace.h>

#include<stdint.h>

static const uint8_t st7735_default_width = 128;
// for 1.44" display
static const uint8_t st7735_default_height_144 = 128;
// for 1.8" display
static const uint8_t st7735_default_height_18 = 160;

enum ST7735_DISPLAY_TYPE {
	ST7735_BLUE,
	ST7735_RED_18_GREENTAB,
	ST7735_RED_18_REDTAB,
	ST7735_RED_18_BLACKTAB,
	ST7735_RED144_GREENTAB,
	ST7735_RED144_JAYCAR
};

static const enum ST7735_DISPLAY_TYPE st7735_type = ST7735_RED144_JAYCAR;


// ST7735 commands
enum ST7735_COMMANDS {
	ST7735_NOP = 0x00,
	ST7735_SWRESET = 0x01,
	ST7735_RDDID = 0x04,
	ST7735_RDDST = 0x09,

	ST7735_SLPIN = 0x10,
	ST7735_SLPOUT = 0x11,
	ST7735_PTLON = 0x12,
	ST7735_NORON = 0x13,

	ST7735_INVOFF = 0x20,
	ST7735_INVON = 0x21,
	ST7735_DISPOFF = 0x28,
	ST7735_DISPON = 0x29,
	ST7735_CASET = 0x2A,
	ST7735_RASET = 0x2B,
	ST7735_RAMWR = 0x2C,
	ST7735_RAMRD = 0x2E,

	ST7735_PTLAR = 0x30,
	ST7735_COLMOD = 0x3A,
	ST7735_MADCTL = 0x36,

	ST7735_FRMCTR1 = 0xB1,
	ST7735_FRMCTR2 = 0xB2,
	ST7735_FRMCTR3 = 0xB3,
	ST7735_INVCTR = 0xB4,
	ST7735_DISSET5 = 0xB6,

	ST7735_PWCTR1 = 0xC0,
	ST7735_PWCTR2 = 0xC1,
	ST7735_PWCTR3 = 0xC2,
	ST7735_PWCTR4 = 0xC3,
	ST7735_PWCTR5 = 0xC4,
	ST7735_VMCTR1 = 0xC5,

	ST7735_RDID1 = 0xDA,
	ST7735_RDID2 = 0xDB,
	ST7735_RDID3 = 0xDC,
	ST7735_RDID4 = 0xDD,

	ST7735_PWCTR6 = 0xFC,

	ST7735_GMCTRP1 = 0xE0,
	ST7735_GMCTRN1 = 0xE1
};

enum ST7735_COLORS {
	ST7735_COLOR_BLACK = 0x0000,
	ST7735_COLOR_BLUE = 0x001F,
	ST7735_COLOR_RED = 0xF800,
	ST7735_COLOR_GREEN = 0x07E0,
	ST7735_COLOR_CYAN = 0x07FF,
	ST7735_COLOR_MAGENTA = 0xF81F,
	ST7735_COLOR_YELLOW = 0xFFE0,
	ST7735_COLOR_WHITE = 0xFFFF
};

enum ST7735_ORIENTATION {
	ST7735_LANDSCAPE,
	ST7735_PORTRAIT,
	ST7735_LANDSCAPE_INV,
	ST7735_PORTRAIT_INV
};

void st7735_init(void);

void st7735_set_orientation(enum ST7735_ORIENTATION orientation);

void st7735_draw_pixel(int16_t x, int16_t y, uint16_t color);
void st7735_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

static inline void st7735_draw_fast_vline(uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
	st7735_fill_rect(x, y, 1, h, color);
}

static inline void st7735_draw_fast_hline(uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
	st7735_fill_rect(x, y, w, 1, color);
}

static inline uint16_t st7735_color(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void st7735_draw_bitmap(uint8_t x, uint8_t y, PGM_P bitmap);

void st7735_draw_mono_bitmap(uint8_t x, uint8_t y, PGM_P bitmap, uint16_t color_set, uint16_t color_unset);

#endif
