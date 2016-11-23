#include "includes.h"

void hal_system_reset(void)
{
    system_reset();
}

void hal_get_device_id(uint32_t * DeviceID)
{
    get_device_id(DeviceID);
}

void hal_get_device_revision(uint32_t * DeviceRevision)
{
    get_device_revision(DeviceRevision);
}

void hal_get_flash_size(uint32_t * FlashSize)
{
    get_flash_size(FlashSize);
}