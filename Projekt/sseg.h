/*
 * sseg.h
 *
 * Created: 2018-12-06 17:58:43
 *  Author: linds
 */ 


#ifndef SSEG_H_
#define SSEG_H_

void SSEG_init(void);
void SSEG_display_single(uint8_t num);
void SSEG_display(uint16_t val, uint16_t max);

#endif /* SSEG_H_ */