#ifndef _SPI_H_
#define _SPI_H_

#include<avr/io.h>

void spi_init(void);

static inline void spi_write(uint8_t byte) {
	SPDR = byte;
	while(!(SPSR & (1<<SPIF)));
}

static inline void spi_set_cs(void) {
	PORTB |= (1 << PB2);
}

static inline void spi_unset_cs(void) {
	PORTB &= ~(1 << PB2);
}


#endif
