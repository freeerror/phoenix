#ifndef __HAL_EEPROM_H__
#define __HAL_EEPROM_H__

void hal_eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length);
FLASH_Status hal_eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length);

#endif