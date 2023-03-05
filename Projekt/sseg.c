
#include <avr/io.h>
void SSEG_init(void){
	//TIMER
	TCCR0A = (1<<WGM01);	// CTC
	TCCR0B = (1<<CS02);		// /256
	TIMSK0 = (1<<OCIE0A);
	OCR0A = 50;				// f = 1/(f_cpu/256 * 50)
	
	//PORT C, 7-seg ANODER
	DDRC = 0xff;
	PORTC = 0;

	//PORTD, 7-seg KATODER
	DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD4);
	PORTD |= (1<<PD4);
}

void SSEG_display_digit(uint8_t num){
		switch (num){
			case 0:
				PORTC = 0b11111100;
				break;
			case 1:
				PORTC = 0b01100000;
				break;
			case 2:
				PORTC = 0b11011010;
				break;
			case 3:
				PORTC = 0b11110010;
				break;
			case 4:
				PORTC = 0b01100110;
				break;
			case 5:
				PORTC = 0b10110110;
				break;
			case 6:
				PORTC = 0b10111110;
				break;
			case 7:
				PORTC = 0b11100000;
				break;
			case 8:
				PORTC = 0b11111110;
				break;
			case 9:
				PORTC = 0b11110110;
				break;
			case '-':
				PORTC = 0b00000010;
				break;
			default:
				PORTC = 0;
		}
}

void SSEG_display(uint16_t val, uint16_t max){
	uint8_t hundreds = 0;
	uint8_t tens = 0;
	uint8_t ones = 0;
	
	if (val < max){
		hundreds = val/100;
		tens = (val%100)/10;
		ones = ((val%100)%10)/1;
	}
	else if (val >= max){
		hundreds = '-';
		tens = '-';
		ones = '-';
	}

	if (PORTD & 0b00010000){
		SSEG_display_digit(tens);
		PORTD &= ~(0b00010000);
		PORTD |= 0b00001000;
	}
	else if (PORTD & 0b00001000){
		SSEG_display_digit(hundreds);
		PORTD &= ~(0b00001000);
		PORTD |= 0b00000100;
	}
	else if (PORTD & 0b00000100){
		SSEG_display_digit(ones);
		PORTD &= ~(0b00000100);
		PORTD |= 0b00010000;
	}
}