#ifndef __HAL_PLATFORM_H__
#define __HAL_PLATFORM_H__

void hal_system_reset(void);
void hal_get_device_id(uint32_t *DeviceID);
void hal_get_device_revision(uint32_t *DeviceRevision);
void hal_get_flash_size(uint32_t *FlashSize);

#endif
