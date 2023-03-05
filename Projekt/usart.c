#include "global_defs.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

void USART_init(void){
	DDRD |= (1<<PD1);		// TXD pin output
	PORTD &= ~(1<<PD1);

	UBRR0 = 12; //Set baud rate = 38.4k
	
	UCSR0A = 0;
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0) | (0<<TXCIE0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	//8 bits data, no parity, 1 stop bit
}

void USART_transmit(char data){
	while(bit_is_clear(UCSR0A, UDRE0)){
	}
	UDR0 = data;
}

void USART_send_string(char *s){
	while (*s != '\0'){
		USART_transmit(*s);
		s++;
	}
}