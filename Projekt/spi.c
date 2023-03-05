#include <avr/io.h>
#include <stdio.h>

void SPI_init(void){
	/* Set MOSI, SCK, SS */
	DDRB |= (1<<PB5)|(1<<PB7)|(1<<PB4);
	/* Enable SPI, Master, set clock rate*/
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<CPOL)|(1<<CPHA);
}

void SPI_transmit(char data){
	/* Start transmission */
	SPDR = data;
	
	//Wait for SPIF flag
	while (!(SPSR & (1<<SPIF))){
	}
}