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

void hal_get_unique_id(uint8_t * UniqueID)
{

    get_unique_id(UniqueID);
}

void hal_get_boot_index(uint8_t * BootIndex)
{
    get_boot_index(BootIndex);
}

void hal_get_version(uint32_t * Version)
{
    get_version(Version);
}