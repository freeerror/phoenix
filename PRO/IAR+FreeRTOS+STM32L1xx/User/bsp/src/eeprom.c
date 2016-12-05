#include "includes.h"

void eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
{
	uint8_t *raddr;

	raddr = (uint8_t *)(EEPROM_BASE_ADDR + addr);

	while(length--)
	{
		*buffer++ = *raddr++;
	}
}

FLASH_Status eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
{
    FLASH_Status status;

    uint32_t waddr;
    
	DATA_EEPROM_Unlock();
    DATA_EEPROM_FixedTimeProgramCmd(ENABLE);

    waddr = EEPROM_BASE_ADDR + addr;

    while(length--)
    {
        status = DATA_EEPROM_ProgramByte(waddr++,*buffer++);

        if(status != FLASH_COMPLETE)
        {
            DATA_EEPROM_Lock();
            return status;
        }
    }

    DATA_EEPROM_Lock();
    return status;
}



