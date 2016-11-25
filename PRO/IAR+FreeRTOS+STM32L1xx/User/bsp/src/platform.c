#include "includes.h"

void system_reset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

void get_device_id(uint32_t *DeviceID)
{
    *DeviceID = (*((__IO uint32_t *)0xE0042000)) & IDCODE_DEVID_MASK;
}

void get_device_revision(uint32_t * DeviceRevision)
{
    *DeviceRevision = (*((__IO uint32_t *)0xE0042000)) >> 16;
}

void get_flash_size(uint32_t * FlashSize)
{
    uint32_t device_id;
    uint16_t temp;

    get_device_id(&device_id);
    
    if((device_id == 0x416) || (device_id == 0x429)) //Cat.1 or Car.2
    {
        temp = (uint16_t)(*((__IO uint32_t *)0x1FF8004C));
    }
    else //Cat.3 or Cat.4 or Cat.5 or Cat.6
    {
        temp = (uint16_t)(*((__IO uint32_t *)0x1FF800CC));
    }

    switch(device_id)
    {
        case 0x416:
        case 0x427:
        case 0x429:
        case 0x437:
            *FlashSize = temp;
            break;
        case 0x436:
            if(temp)
            {
                *FlashSize = 256;
            }
            else
            {
                *FlashSize = 384;
            }
            break;
        default:
            break;
    }
    *FlashSize = (*FlashSize) * 1024;
}

void get_unique_id(uint8_t * UniqueID)
{
    uint8_t i;
    uint32_t device_id;
    uint8_t temp[12]; //96bit unique id

    get_device_id(&device_id);
    
    if((device_id == 0x416) || (device_id == 0x429)) //Cat.1 or Car.2
    {
        
    }
    else //Cat.3 or Cat.4 or Cat.5 or Cat.6
    {
        for(i = 0;i < 12;i++)
        {
            temp[i] = *((__IO uint32_t *)(0x1FF800D0 + i));
        }
    }

    memcpy(UniqueID,temp,12);
}

void get_boot_index(uint8_t * BootIndex)
{
    uint32_t temp;
    temp = SYSCFG_GetBootMode();
    switch(temp)
    {
        case 0x00000000: //Main flash memory
            *BootIndex = 0; 
            break;

        case 0x00000100: //System flash memory
            *BootIndex = 1;
            break;

        case 0x00000200: //FSMC memory
            *BootIndex = 2;
            break;

        case 0x00000300: //Embedded SRAM memory
            *BootIndex = 3;
            break;

        default:
            break;
    }
}

void get_version(uint32_t * Version)
{
    *Version = VERSION;
}

