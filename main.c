#include<avr/io.h>
#include<util/delay.h>

#include "spi.h"
#include "st7735.h"
#include "st7735_gfx.h"
#include "st7735_font.h"

//#include "logo.h"
#include "logo_bw.h"

//#include "tom_thumb.h"
#include "free_sans.h"

int main(void) {
    spi_init();
    st7735_init();

    st7735_set_orientation(ST7735_LANDSCAPE);
    st7735_fill_rect(0, 0, 160, 128, ST7735_COLOR_BLACK);

    st7735_draw_mono_bitmap(16, 4, &logo_bw, ST7735_COLOR_WHITE, ST7735_COLOR_BLACK);

    //st7735_draw_bitmap(10, 10, &logo);


    for(uint8_t x = 8; x <= 160; x += 8) {
        st7735_draw_line(0, 0, x, 128, ST7735_COLOR_GREEN);
    }

    for(uint8_t y = 8; y <= 128; y += 8) {
        st7735_draw_line(0, 0, 160, y, ST7735_COLOR_GREEN);
    }

    for(uint8_t r = 8; r <= 80; r += 8) {
        st7735_draw_circle(80, 64, r, ST7735_COLOR_RED);
    }

    for(uint8_t r = 8; r <= 160; r += 8) {
        st7735_draw_circle(0, 0, r, ST7735_COLOR_BLUE);
    }

    uint8_t h = 16;
    uint8_t w = 20;
    for(uint8_t i = 0; i < 8; i++) {
        st7735_draw_rect(80 - w / 2, 64 - h / 2, w, h, ST7735_COLOR_YELLOW);
        h += 16;
        w += 20;
    }

    _delay_ms(5000);
    st7735_fill_rect(0, 0, 160, 128, ST7735_COLOR_BLACK);

    st7735_draw_text(5, 30, "This is\njust a Test xyz\nST7735\nLongHairedHacker", &FreeSans, 1, ST7735_COLOR_CYAN);
    st7735_draw_fast_hline(10, 30, 100, ST7735_COLOR_RED);

    while(1);
}
