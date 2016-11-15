#ifndef __SPI_H__
#define __SPI_H__

#include "sys.h"

void SPI2_Init(void);
//uint8_t spi2_read_write_byte(uint8_t dat);
//uint8_t spi2_read_write_byte(void);

extern uint8_t spi2_read_write_byte(uint8_t tx_data);

#endif
