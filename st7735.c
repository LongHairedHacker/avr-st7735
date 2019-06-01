#include "st7735.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "spi.h"
#include "st7735initcmds.h"

uint8_t st7735_screen_row_start = 0;
uint8_t st7735_screen_column_start = 0;
uint8_t st7735_row_start = 0;
uint8_t st7735_column_start = 0;
uint8_t st7735_width = 0;
uint8_t st7735_height = 0;
enum ST7735_ORIENTATION st7735_orientation = ST7735_LANDSCAPE;

static inline void st7735_set_rs(void) {
	PORTD |= (1 << PD6);
}

static inline void st7735_unset_rs(void) {
	PORTD &= ~(1 << PD6);
}

static inline void st7735_set_rst(void) {
	PORTD |= (1 << PD7);
}

static inline void st7735_unset_rst(void) {
	PORTD &= ~(1 << PD7);
}

static inline void st7735_write_cmd(enum ST7735_COMMANDS cmd) {
	st7735_unset_rs();

	spi_unset_cs();
	spi_write(cmd);
	spi_set_cs();
}

static inline void st7735_write_data(uint8_t data) {
	st7735_set_rs();

	spi_unset_cs();
	spi_write(data);
	spi_set_cs();
}

static inline void st7735_write_color(uint16_t color) {
	spi_write(color >> 8);
	spi_write(color);
}


static inline void st7735_reset(void) {
	spi_unset_cs();
	st7735_set_rst();
	_delay_ms(500);
	st7735_unset_rst();
	_delay_ms(500);
	st7735_set_rst();
	_delay_ms(500);
}


void st7735_run_command_list(const uint8_t *addr) {
	uint8_t  cmd_count, arg_count, has_delay;

	cmd_count = pgm_read_byte(addr++);   // Number of commands to follow
	for(uint8_t cmd_pos = 0; cmd_pos < cmd_count; cmd_pos++) {
		st7735_write_cmd(pgm_read_byte(addr++)); 	// Read, send command
		arg_count  = pgm_read_byte(addr++);    		// Number of args to follow
		has_delay = arg_count & DELAY_FLAG;         // If set, delay follows args
		arg_count &= ~DELAY_FLAG;                  	// Number of args
		for(uint8_t arg_pos = 0; arg_pos < arg_count; arg_pos++) { // For each argument...
			st7735_write_data(pgm_read_byte(addr++));  		// Read, send argument
		}

		if(has_delay) {
			uint8_t ms;
			ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
			if(ms == 255) {  // If 255, delay for 500 ms
				_delay_ms(500);
			}
			else {
				for(int i = 0; i < ms; i += 5) {
					_delay_ms(5);
				}
			}

		}
	}
}


void st7735_init() {
	// Set rs and rst output
	DDRD |= (1 << PD6) | (1 << PD7);

	st7735_reset();

	switch(st7735_type) {
		case ST7735_BLUE:
			st7735_run_command_list(st7735_blue_init);
			break;

		case ST7735_RED_18_GREENTAB:
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_green2);
			st7735_run_command_list(st7735_red_init3);
			st7735_column_start = st7735_screen_column_start = 2;
			st7735_row_start = st7735_screen_row_start = 1;
			st7735_width = st7735_default_width;
			st7735_height = st7735_default_height_18;
			break;

		case ST7735_RED_18_REDTAB:
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_red2);
			st7735_run_command_list(st7735_red_init3);
			st7735_width = st7735_default_width;
			st7735_height = st7735_default_height_18;
			break;

		case ST7735_RED_18_BLACKTAB:
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_red2);
			st7735_run_command_list(st7735_red_init3);
			// Change MADCTL color filter for black
			st7735_write_cmd(ST7735_MADCTL);
			st7735_write_data(0xC0);
			st7735_width = st7735_default_width;
			st7735_height = st7735_default_height_18;
			break;

		case ST7735_RED144_GREENTAB:
			st7735_height = st7735_default_height_144;
			st7735_column_start = st7735_screen_column_start = 2;
			st7735_row_start = st7735_screen_row_start = 3;
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_green1442);
			st7735_run_command_list(st7735_red_init3);
			st7735_width = st7735_default_width;
			st7735_height = st7735_default_height_144;
			break;
		case ST7735_RED144_JAYCAR:
			st7735_height = st7735_default_height_144;
			st7735_column_start = st7735_screen_column_start = 0;
			st7735_row_start = st7735_screen_row_start = 32;
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_green1442);
			st7735_run_command_list(st7735_red_init3);
			st7735_width = st7735_default_width;
			st7735_height = st7735_default_height_144;
			break;
	}
}


enum ST7735_MADCTL_ARGS {
	MADCTL_MY = 0x80,	// Mirror Y
	MADCTL_MX = 0x40,	// Mirrror x
	MADCTL_MV = 0x20,	// Swap XY
	MADCTL_ML = 0x10,	// Scan address order
	MADCTL_RGB = 0x00,
	MADCTL_BGR = 0x08,
	MADCTL_MH = 0x04 	// Horizontal scan oder
};

void st7735_set_orientation(enum ST7735_ORIENTATION orientation) {
	st7735_write_cmd(ST7735_MADCTL);

  	switch (orientation) {
		case ST7735_PORTRAIT:
		    if(st7735_type == ST7735_RED_18_BLACKTAB) {
		    	st7735_write_data(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
		    } else {
		    	st7735_write_data(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
		    }

			st7735_width  = st7735_default_width;

		    if(
			    st7735_type == ST7735_RED144_GREENTAB ||
			    st7735_type == ST7735_RED144_JAYCAR
		    ) {
			st7735_height = st7735_default_height_144;
		    } else {
			st7735_height = st7735_default_height_18;
		    }

			st7735_column_start = st7735_screen_column_start;
			st7735_row_start = st7735_screen_row_start;
			break;


		case ST7735_LANDSCAPE:
			if(st7735_type == ST7735_RED_18_BLACKTAB) {
				st7735_write_data(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
			} else {
				st7735_write_data(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
			}

			if(
				st7735_type == ST7735_RED144_GREENTAB ||
				st7735_type == ST7735_RED144_JAYCAR
			) {
				st7735_width = st7735_default_height_144;
			}
			else {
				st7735_width = st7735_default_height_18;
			}

			if(st7735_type == ST7735_RED144_JAYCAR) {
			    st7735_column_start = 32;
			}

			st7735_height = st7735_default_width;
			st7735_column_start = st7735_screen_row_start;
			st7735_row_start = st7735_screen_column_start;
			break;

		case ST7735_PORTRAIT_INV:
			if (st7735_type == ST7735_RED_18_BLACKTAB) {
				st7735_write_data(MADCTL_RGB);
			} else {
				st7735_write_data(MADCTL_BGR);
			}

			st7735_width  = st7735_default_width;

			if(
				st7735_type == ST7735_RED144_GREENTAB ||
				st7735_type == ST7735_RED144_JAYCAR
			) {
				st7735_height = st7735_default_height_144;
			} else {
				st7735_height = st7735_default_height_18;
			}

			if(st7735_type == ST7735_RED144_JAYCAR) {
				st7735_column_start = 0;
			}
			st7735_column_start = st7735_screen_column_start;
			st7735_row_start = st7735_screen_row_start;

			break;

		case ST7735_LANDSCAPE_INV:
			if (st7735_type == ST7735_RED_18_BLACKTAB) {
				st7735_write_data(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
			} else {
				st7735_write_data(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
			}

			if(
				st7735_type == ST7735_RED144_GREENTAB ||
				st7735_type == ST7735_RED144_JAYCAR
			) {
				st7735_width = st7735_default_height_144;
			} else {
				st7735_width = st7735_default_height_18;
			}

			if(st7735_type == ST7735_RED144_JAYCAR) {
				st7735_column_start = 0;
			}

			st7735_height = st7735_default_width;
			st7735_column_start = st7735_screen_row_start;
			st7735_row_start = st7735_screen_column_start;
			break;
	}
}

void st7735_set_addr_win(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	st7735_write_cmd(ST7735_CASET); // Column addr set
	st7735_write_data(0x00);
	st7735_write_data(x0 + st7735_column_start);	// XSTART
	st7735_write_data(0x00);
	st7735_write_data(x1 + st7735_column_start); // XEND

	st7735_write_cmd(ST7735_RASET); // Row addr set
	st7735_write_data(0x00);
	st7735_write_data(y0 + st7735_row_start); // YSTART
	st7735_write_data(0x00);
	st7735_write_data(y1 + st7735_row_start); // YEND

	st7735_write_cmd(ST7735_RAMWR); // write to RAM
}

void st7735_draw_pixel(int16_t x, int16_t y, uint16_t color) {
	if(x < 0 || x >= st7735_width || y < 0 || y >= st7735_height){
		return;
	}

  	st7735_set_addr_win(x, y, x+1, y+1);

  	st7735_set_rs();
  	spi_unset_cs();

	st7735_write_color(color);

  	spi_set_cs();
}


void st7735_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
	if(x >= st7735_width || y >= st7735_height) {
		return;
	}

	if((x + w - 1) >= st7735_width) {
		w = st7735_width  - x;
	}
	if((y + h - 1) >= st7735_height) {
		h = st7735_height - y;
	}

	st7735_set_addr_win(x, y, x + w - 1, y + h - 1);

	st7735_set_rs();
	spi_unset_cs();

	for(uint8_t i = 0; i < h; i++) {
		for(uint8_t j = 0; j < w; j++) {
			st7735_write_color(color);
		}
	}

	spi_set_cs();
}


void st7735_draw_bitmap(uint8_t x, uint8_t y, PGM_P bitmap) {
	uint8_t w = pgm_read_word(bitmap);
	bitmap += 2;
	uint8_t h = pgm_read_word(bitmap);
	bitmap += 2;
	uint8_t max_x = x + w - 1;
	uint8_t max_y = y + h - 1;

	if(x >= st7735_width || y >= st7735_height) {
		return;
	}

	if(max_x >= st7735_width) {
		max_x = st7735_width - 1;
	}

	if(max_y >= st7735_height) {
		max_y = st7735_height - 1;
	}

	st7735_set_addr_win(x, y, max_x, max_y);

	st7735_set_rs();
	spi_unset_cs();

	for(uint8_t i = 0; i < h; i++) {
		for(uint8_t j = 0; j < w; j++) {
			if((x + j) >= st7735_width || (y + i) >= st7735_height) {
				bitmap += 2;
				continue;
			}
			uint16_t color = pgm_read_word(bitmap);
			st7735_write_color(color);
			bitmap += 2;
		}
	}

	spi_set_cs();
}

void st7735_draw_mono_bitmap(uint8_t x, uint8_t y, PGM_P bitmap, uint16_t color_set, uint16_t color_unset) {
	uint8_t w = pgm_read_byte(bitmap++);
	uint8_t h = pgm_read_byte(bitmap++);
	uint8_t max_x = x + w - 1;
	uint8_t max_y = y + h - 1;

	if(x >= st7735_width || y >= st7735_height) {
		return;
	}

	if(max_x >= st7735_width) {
		max_x = st7735_width - 1;
	}

	if(max_y >= st7735_height) {
		max_y = st7735_height - 1;
	}

	st7735_set_addr_win(x, y, max_x, max_y);

	st7735_set_rs();
	spi_unset_cs();

	uint16_t bit_pos = 0;
	uint8_t byte = 0;
	for(uint8_t i = 0; i < h; i++) {
		for(uint8_t j = 0; j < w; j++) {
			if(bit_pos % 8 == 0) {
				byte = pgm_read_byte(bitmap++);
			}

			if((x + j) >= st7735_width || (y + i) >= st7735_height) {
				bit_pos++;
				continue;
			}
			if(byte & (1 << (bit_pos % 8))) {
				st7735_write_color(color_set);
			}
			else {
				st7735_write_color(color_unset);
			}
			bit_pos++;
		}
	}

	spi_set_cs();
}
