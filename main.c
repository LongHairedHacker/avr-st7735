#include<avr/io.h>
#include<util/delay.h>

#include "spi.h"
#include "st7735.h"

#include "logo.h"
#include "logo_bw.h"

int main(void) {
    spi_init();
    st7735_init();

    st7735_set_orientation(ST7735_LANDSCAPE);
    st7735_fill_rect(0, 0, 160, 128, ST7735_COLOR_BLACK);

    st7735_draw_mono_bitmap(0, 0, &logo_bw, ST7735_COLOR_WHITE, ST7735_COLOR_BLACK);

    st7735_draw_bitmap(10, 10, &logo);

    while(1);
}
