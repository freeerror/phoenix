#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
	 
void delay_init(void);
void delay_n62_5ns(uint16_t nns);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif





























