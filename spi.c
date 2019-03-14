#include "spi.h"

void spi_init(void) {
	// Set MOSI and SCK, SS/CS output, all others input
	DDRB = (1<<PB3) | (1<<PB5) | (1<<PB2);
	// Enable SPI, Master, set clock rate fck/4, mode 0
	SPCR = (1<<SPE) | (1<<MSTR);

	// Set SS/CS
	PORTB |= (1 << PB2);
}
