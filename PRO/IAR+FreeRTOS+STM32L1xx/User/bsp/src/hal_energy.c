#include "includes.h"

void hal_get_power_voltage(uint16_t *power_voltage)
{
    power_adc_conversion(power_voltage);
}

