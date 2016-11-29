#ifndef __EEPROM_H__
#define __EEPROM_H__

//可调用FLASH_Status DATA_EEPROM_ProgramByte(uint32_t Address, uint8_t Data)；
//调用前先调用DATA_EEPROM_Unlock()；解除eeprom的锁定
//再调用DATA_EEPROM_FixedTimeProgramCmd()；写之前先自动擦除
//最后再调用DATA_EEPROM_Lock()；锁定eeprom，防止误操作。

#endif
