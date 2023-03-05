/*
 * ad.h
 *
 * Created: 2018-11-20 11:37:55
 *  Author: linds
 */ 


#ifndef AD_H_
#define AD_H_

void AD_init(char mode);
uint16_t AD_read(char ch);
uint16_t AD_read2(char ch);

#endif /* AD_H_ */