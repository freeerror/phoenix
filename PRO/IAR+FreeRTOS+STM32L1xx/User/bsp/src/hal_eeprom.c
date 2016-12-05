#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名:void hal_eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
*	功能说明:按字节读取MCU内部的eeprom内的数据
*	形    参:uint16_t addr:需要读取的内部eeprom的地址，范围:0x0000 - 0x2000
*            uint8_t *buffer:存放读取出来的数据的缓冲区
*            uint16_t length:需要读取的数据的个数，单位:byte
*	返 回 值:无 
*********************************************************************************************************
*/
void hal_eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
{
    eeprom_read_bytes(addr,buffer,length);
}

/*
*********************************************************************************************************
*	函 数 名:FLASH_Status hal_eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
*	功能说明:按字节把数据写入MCU内部的eeprom
*	形    参:uint16_t addr:需要写入内部eeprom的起始地址，范围:0x0000 - 0x2000
*            uint8_t *buffer:存放写入数据的缓冲区
*            uint16_t length:需要写入的数据的个数，单位:byte
*	返 回 值:无 
*********************************************************************************************************
*/
FLASH_Status hal_eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
{
    FLASH_Status status;

    status = eeprom_write_bytes(addr,buffer,length);

    return status;
}

