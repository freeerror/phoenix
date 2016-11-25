#ifndef __HAL_PLATFORM_H__
#define __HAL_PLATFORM_H__

void hal_system_reset(void);
void hal_get_device_id(uint32_t *DeviceID);
void hal_get_device_revision(uint32_t *DeviceRevision);
void hal_get_flash_size(uint32_t *FlashSize);
void hal_get_unique_id(uint8_t *UniqueID);
void hal_get_boot_index(uint8_t *BootIndex);
void hal_get_version(uint32_t *Version);

#endif
