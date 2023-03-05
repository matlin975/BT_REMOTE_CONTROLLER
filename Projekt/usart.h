/*
 * usart.h
 *
 * Created: 2018-11-14 12:17:57
 *  Author: linds
 */ 


#ifndef USART_H_
#define USART_H_

void USART_init(void);
void USART_transmit(char data);
void USART_send_string(char *s);

#endif /* USART_H_ */