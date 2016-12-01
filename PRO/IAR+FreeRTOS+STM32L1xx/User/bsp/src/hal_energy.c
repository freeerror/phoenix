#include "includes.h"

void hal_get_power_voltage(float *power_voltage)
{
    power_adc_conversion(power_voltage);
}

