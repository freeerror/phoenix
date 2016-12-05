#include "includes.h"

void delay_us(uint16_t nus)
{
    uint16_t i;

    for(i = 0;i < nus;i++)
    {
        ;;
    }
}

void delay_ms(uint16_t nms)
{
    uint16_t i;

    for(i = 0;i < nms;i++)
    {
        delay_us(1000);
    }
}
