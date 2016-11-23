#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#define IDCODE_DEVID_MASK 0x00000FFF

void system_reset(void);
void get_device_id(uint32_t *DeviceID);
void get_device_revision(uint32_t *DeviceRevision);
void get_flash_size(uint32_t *FlashSize);
void get_unique_id(uint8_t *UniqueID);

#endif