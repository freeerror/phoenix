#include "SX1276.h"
#include "SPI.h"

static uint8_t sx1276_read_one_byte(uint8_t addr)
{
	uint8_t byte;
	
	sx1276_cs = 0;
	spi2_read_write_byte(addr);
	byte = spi2_read_write_byte(0xFF);
	sx1276_cs = 1;
	
	return(byte);
}

static void sx1276_write_one_byte(uint8_t addr,uint8_t dat)
{
	sx1276_cs = 0;
	spi2_read_write_byte(addr | 0x80);
	spi2_read_write_byte(dat);
	sx1276_cs = 1;
}

uint8_t sx1276_read_version(uint8_t reg_add)
{
	return sx1276_read_one_byte(reg_add);
}
