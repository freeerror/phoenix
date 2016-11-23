#include "includes.h"

void system_reset(void)
{    
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
