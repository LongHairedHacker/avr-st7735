#include<avr/io.h>
#include<util/delay.h>

#include "spi.h"
#include "st7735.h"

void main() {
    spi_init();
    st7735_init();




    st7735_set_orientation(ST7735_LANDSCAPE);
    st7735_fill_rect(0, 0, 160, 128, ST7735_COLOR_GREEN);
    st7735_fill_rect(0, 0, 80, 127, ST7735_COLOR_RED);
    st7735_fill_rect(0, 0, 80, 64, ST7735_COLOR_CYAN);
    st7735_draw_fast_hline(80, 64, 40, ST7735_COLOR_BLUE);
    st7735_draw_fast_vline(120, 64, 32, ST7735_COLOR_BLUE);

}
