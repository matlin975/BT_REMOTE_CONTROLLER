#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "global_defs.h"
#include "sseg.h"
#include "usart.h"
#include "ad.h"
#include "spi.h"
#include "oled.h"

//GLOBAL VARIABLES
volatile uint16_t joy_left = 0;
volatile uint16_t joy_right = 0;
volatile uint16_t vehicle_distance = 0;
volatile uint16_t connection_status = 0;
volatile uint16_t vehicle_current = 0;

//COMMUNICATION VARIABLES
volatile uint8_t data_in[8];
char command_in[8];
volatile uint8_t data_count, command_ready;
volatile uint8_t ack_flag = 0;
volatile uint8_t count = 0;
volatile uint8_t timeout = 0;

//COMMUNICATION FUNCTIONS
void RN42_connect(void);
void send_command(char *s){
		int x = 1;
		while (*s != '\0'){
			if (x == 1){
				USART_transmit(*s);
				x = 0;
			}
			else{
				USART_transmit(*s);
			}
			s++;
		}
}
void copy_command(void){
	cli();
	for (int i = 0; i<8; i++){
		command_in[i] = data_in[i];
	}
	sei();
}
void send_value (uint16_t value){
	char buffer[8];
	itoa(value, buffer, 10);
	USART_send_string(buffer);
}
static uint16_t parse_assignment(){
	char *pch;
	char cmdValue[16];
	pch = strchr(command_in, '=');
	strcpy(cmdValue, pch+1);
	return atoi(cmdValue);
}
void process_command(){
	char error = 0;
	
	switch (command_in[0]){
		case 'C':
			if (command_in[1] == '?'){
				send_command("d?\n");
			}
			else if (command_in[1] == '='){
				connection_status = parse_assignment();
			}
			else{
				error = 1;
			}
			break;
		case 'd':
			if (command_in[1] == '=')
			{
				vehicle_distance = parse_assignment();
				vehicle_distance = vehicle_distance/1.4;
				vehicle_distance =  (6787/(vehicle_distance-3))-4;

				send_command("I?\n");
			}
			else{
				error = 1;
			}
			break;
		case 'I':
			if (command_in[1] == '=')
			{
				vehicle_current = parse_assignment();
				send_command("C?\n");
			}
			else{
				error = 1;
			}
			break;
		case 'V':
			if (command_in[1] == '?'){
					send_command("V=");
					send_value(joy_left);
					USART_transmit('\n');
			}
			else{
				error = 1;
			}
			break;
		 case 'D':
			if (command_in[1] == '?'){
					send_command("D=");
					send_value(joy_right);
					USART_transmit('\n');	
			}
			else{
				error = 1;
			}
			break;
		default:
			error = 1;
			break;

	}
	if (error == 1){
		//flash red led
		PORTB |= (1<<PB1);
		_delay_ms(LED_BLINK_TIME);
		PORTB &= ~(1<<PB1);
	}
}

int main(void)
{
//----------------------------------------------------------------------
//---------------------------------INIT---------------------------------
//----------------------------------------------------------------------
	//STATUS LEDS
	DDRB |= (1<<PB0)|(1<<PB1);
	PORTB = 0;
	
	//PERIPHERALS
	SSEG_init();
	USART_init();
	AD_init(0); //0 = single-conversion mode
	SPI_init();
	OLED_init();
	
	//BLUETOOTH
	RN42_connect(); //Connects to the vehicle over bluetooth
	
	//TIMER1 (CONNECTION TIMEOUT-TIMER)
	TCCR1B = (1<<CS12)|(1<<WGM12); //prescaler == /256, 	WGM 12 = CTC
	TIMSK1 = (1<<OCIE1A);
	OCR1A = 20000;
	
	sei();
	
//----------------------------------------------------------------------
//-----------------------------PROGRAM LOOP-----------------------------
//----------------------------------------------------------------------
    while (1) 
    {	
		//If an acknowledgement is received from the vehicle after sending a command
		if (ack_flag == TRUE){
			//flash green led
			PORTB |= (1<<PB0);
			_delay_ms(LED_BLINK_TIME);
			PORTB &= ~(1<<PB0);

			ack_flag = FALSE;
		}
		
		//If a command is received and ready to be processed
		if(command_ready == 1){
			copy_command();
			process_command();
			command_ready = 0;
		}
		
		//Read joystick positions
		joy_left = 1023-AD_read(0);
		joy_right = AD_read(1);

		//Update OLED-display
		OLED_update();
    }
}



//-------------------------------------------------------------------
//----------------------INTERRUPT ROUTINES---------------------------
//-------------------------------------------------------------------
//Data received interrupt
ISR (USART0_RX_vect){
	char data = UDR0;
	
	//If ACK-bit is received
	if ((data & ACK) == ACK){
		TCNT1 = 0; //Reset timeout-timer
		timeout = 0;
		
		ack_flag = TRUE;
		data -= ACK; //Remove ack-bit from char
	}
	
	data_in[data_count] = data;
	
	if (data_in[data_count] == '\n'){
		command_ready = 1;
		data_count = 0;
	} else {
		data_count++;
	}
}

//SSEG-display multiplexing
ISR(TIMER0_COMPA_vect){
	SSEG_display(vehicle_distance , MAX_RANGE);
}

//Connection time-out interrupt
ISR(TIMER1_COMPA_vect){
	//flash red led
	PORTB |= (1<<PB1);
	_delay_ms(LED_BLINK_TIME);
	PORTB &= ~(1<<PB1);
	
	//Ping connection to vehicle
	send_command("C?\n");
	timeout++;
		
	//Try to reconnect
	if (timeout >= 8){
		RN42_connect();
		timeout = 0;
	}
}



//-------------------------------------------------------------------
//-----------------------FUNCTION DEFINITIONS------------------------
//-------------------------------------------------------------------
//Connects to the vehicles bluetooth-module
void RN42_connect(void){
	_delay_ms(100);
	USART_send_string("$$$");
	_delay_ms(100);
	USART_send_string("C");
	_delay_ms(100);
	USART_send_string("\n");
	_delay_ms(2000);
	USART_send_string("---");
	_delay_ms(100);
	USART_send_string("\n");
	_delay_ms(1000);
}