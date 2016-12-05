#include "st7735.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "spi.h"
#include "st7735initcmds.h"

static unit8_t st7735_row_start = 0;
static unit8_t st7735_column_start = 0;
static uint8_t st7735_height = st7735_height_18;

static inline void st7735_set_rs() {
	PORTB |= (1 << PB1);
}

static inline void st7735_unset_rs() {
	PORTB &= ~(1 << PB1);
}

static inline void st7735_set_rst() {
	PORTB |= (1 << PB0);
}

static inline void st7735_unset_rst() {
	PORTB &= ~(1 << PB0);
}


static inline void st7735_write_cmd(enum ST7735_COMMANDS cmd) {
	st7735_unset_rs();

	spi_unset_cs();
	spi_write(cmd);
	spi_set_cs();
}

static inline void st7735_write_data(enum ST7735_COMMANDS cmd) {
	st7735_set_rs();

	spi_unset_cs();
	spi_write(cmd);
	spi_set_cs();
}


static inline void st7735_reset() {
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
				_delay_ms(ms);
			}

		}
	}
}


void st7735_init() {
	// Set rs and rst output
	DDRB |= (1 << PB1) | (1 << PB0);

	st7735_reset();

	switch(st7735_type) {
		case ST7735_BLUE:
			st7735_run_command_list(st7735_blue_init);
			break;

		case ST7735_RED_18_GREENTAB:
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_green2);
			st7735_run_command_list(st7735_red_init3);
			st7735_column_start = 2;
			st7735_row_start = 1;
			break;

		case ST7735_RED_18_REDTAB:
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_red2);
			st7735_run_command_list(st7735_red_init3);
			break;

		case ST7735_RED_18_BLACKTAB:
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_red2);
			st7735_run_command_list(st7735_red_init3);
			// Change MADCTL color filter for black
			st7735_write_cmd(ST7735_MADCTL);
			st7735_write_data(0xC0);
			break;

		case ST7735_RED144_GREENTAB:
			st7735_run_command_list(st7735_red_init1);
			st7735_run_command_list(st7735_red_init_green1442);
			st7735_run_command_list(st7735_red_init3);
			st7735_height = st7735_height_144;
			st7735_column_start = 2;
			st7735_row_start = 3;
			break;
	}


}
