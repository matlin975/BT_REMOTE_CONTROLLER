
#ifndef GLOBAL_DEFS_H_
#define GLOBAL_DEFS_H_

#include <avr/io.h>

#define F_CPU 8000000UL
#define LED_BLINK_TIME 5
#define ACK 0b10000000
#define NACK 0
#define MAX_RANGE 120

#define TRUE 1
#define FALSE 0

extern volatile uint16_t joy_left;
extern volatile uint16_t joy_right;
extern volatile uint16_t vehicle_current;
extern volatile uint8_t timeout;

#endif /* GLOBAL_DEFS_H_ */

