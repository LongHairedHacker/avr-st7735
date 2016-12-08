#include "st7735_font.h"

#include <string.h>
#include <avr/pgmspace.h>

#include "st7735.h"



void st7735_draw_char(int16_t x, int16_t y, const GFXglyph *glyph, const GFXfont *font,
                        uint8_t size, uint16_t color) {
    if(size < 1) {
        return;
    }

    // Clipping will be done by fill rect if necessary

    uint8_t  *bitmap = font->bitmap;
    uint16_t bo = glyph->bitmapOffset;
    uint8_t bits = 0, bit = 0;
    uint16_t set_pixels = 0;

    uint8_t  cur_x, cur_y;
    for(cur_y = 0; cur_y < glyph->height; cur_y++) {
        for(cur_x = 0; cur_x < glyph->width; cur_x++) {
            if(bit == 0) {
                bits = pgm_read_byte(&bitmap[bo++]);
                bit  = 0x80;
            }

            if(bits & bit) {
                set_pixels++;
            } else if (set_pixels > 0) {
                st7735_fill_rect(x + (glyph->xOffset + cur_x-set_pixels) * size,
                                y + (glyph->yOffset+cur_y) * size,
                                size * set_pixels,
                                size,
                                color);
                set_pixels=0;
            }

            bit >>= 1;
        }

        // Draw rest of line
        if (set_pixels > 0) {
            st7735_fill_rect(x + (glyph->xOffset + cur_x-set_pixels) * size,
                            y + (glyph->yOffset + cur_y) * size,
                            size * set_pixels,
                            size,
                            color);
            set_pixels=0;
        }
    }
}


void st7735_draw_text(int8_t x, int8_t y, char *text, const GFXfont *p_font,
                        uint8_t size, uint16_t color) {

    int16_t cursor_x = x;
    int16_t cursor_y = y;

    GFXfont font;
    memcpy_P(&font, p_font, sizeof(GFXfont));


    for(uint16_t text_pos = 0; text_pos < strlen(text); text_pos++) {
        char c = text[text_pos];

        if(c == '\n') {
            cursor_x = x;
            cursor_y += font.yAdvance * size;
        }
        else if(c >= font.first && c <= font.last && c != '\r') {
            GFXglyph glyph;
            memcpy_P(&glyph, &font.glyph[c - font.first], sizeof(GFXglyph));

            st7735_draw_char(cursor_x, cursor_y, &glyph, &font, size, color);
            cursor_x += glyph.xAdvance * size;
        }

    }

}
