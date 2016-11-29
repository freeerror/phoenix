#ifndef __PLATFORM_H__
#define __PLATFORM_H__

//stm32l162rdt6 --- 384k flash, 48k ram, 12k eeprom cat.4
//stm32l151vct6 --- 256k flash, 32k ram, 8k eeprom cat.3

#define VERSION 16112301
#define IDCODE_DEVID_MASK 0x00000FFF

void system_reset(void);
void get_device_id(uint32_t *DeviceID);
void get_device_revision(uint32_t *DeviceRevision);
void get_flash_size(uint32_t *FlashSize);
void get_unique_id(uint8_t *UniqueID);
void get_boot_index(uint8_t *BootIndex);
void get_version(uint32_t *Version);

#endif