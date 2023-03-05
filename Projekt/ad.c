/*
 * ad.c
 *
 * Created: 2018-11-20 11:37:43
 *  Author: linds
 */ 
#include <avr/io.h>
#include "ad.h"
#include "global_defs.h"

void AD_init(char mode){
	
	//SINGLE CONVESION
	if (mode == 0){
		PRR0 &= ~(1<<PRADC);
		//ADMUX -- REFS1, REFS0,    ADLAR,      MUX4, MUX3, MUX2, MUX1, MUX0		s.333
		//         Voltage ref     l adjust            Channel select
		//             01             1                     x
		ADMUX = (1<<REFS0)|(0<<ADLAR);
	
		//ADCSRA--     ADEN,           ADSC,              ADATE,                ADIF,                ADIE,            ADPS2, ADPS1, ADPS0        s.334
		//          ADC Enable   Start conversion    Auto trigger en       Interrupt flag      Interrupt enable            Prescaler
		ADCSRA = (1<<ADEN)|(0<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADSC)|(0<<ADIE);
	
	}
	
	//AUTO-TRIGGER
	else if(mode == 1){
		PRR0 &= ~(1<<PRADC);
		//ADMUX -- REFS1, REFS0,    ADLAR,      MUX4, MUX3, MUX2, MUX1, MUX0		s.333
		//         Voltage ref     l adjust            Channel select
		//             01             1                     x
		ADMUX = (1<<REFS0)|(0<<ADLAR);
				
		//ADCSRA--     ADEN,           ADSC,              ADATE,                ADIF,                ADIE,            ADPS2, ADPS1, ADPS0        s.334
		//          ADC Enable   Start conversion    Auto trigger en       Interrupt flag      Interrupt enable            Prescaler
		ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADSC);
				
		//ADCSRB--     ACME,         ADTS2,       ADTS1,        ADTS0
		//       Comp mux enable			auto trigger source
		//ADCSRB = 000 = free running
	}
	
	//INTERRUPT
	else if(mode == 2){
		PRR0 &= ~(1<<PRADC);
		//ADMUX -- REFS1, REFS0,    ADLAR,      MUX4, MUX3, MUX2, MUX1, MUX0		s.333
		//         Voltage ref     l adjust            Channel select
		//             01             1                     x
		ADMUX = (1<<REFS0)|(0<<ADLAR);
				
		//ADCSRA--     ADEN,           ADSC,              ADATE,                ADIF,                ADIE,            ADPS2, ADPS1, ADPS0        s.334
		//          ADC Enable   Start conversion    Auto trigger en       Interrupt flag      Interrupt enable            Prescaler
		ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADSC)|(1<<ADIE);
				
		//ADCSRB--     ACME,         ADTS2,       ADTS1,        ADTS0
		//       Comp mux enable			auto trigger source
		ADCSRB |= (0<<ADTS2)|(1<<ADTS1)|(1<<ADTS0);
	}
}

uint16_t AD_read(char ch){
	ADMUX &= ~(1<<MUX4)&~(1<<MUX3)&~(1<<MUX2)&~(1<<MUX1)&~(1<<MUX0);
	ADMUX |= ch;

	ADCSRA |= (1<<ADSC);
	
	while ((ADCSRA & (1<<ADSC))){
	}

	return ADC;
}

uint16_t AD_read2(char ch){
	PORTB |= (1<<PB0);
	
	ADMUX &= ~(1<<MUX4)&~(1<<MUX3)&~(1<<MUX2)&~(1<<MUX1)&~(1<<MUX0);
	ADMUX |= ch;
	
	PORTB &= ~(1<<PB0);
	return ADC;
}
