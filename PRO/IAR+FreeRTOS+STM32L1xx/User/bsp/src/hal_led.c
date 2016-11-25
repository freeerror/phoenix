#include "includes.h"

void hal_set_led(ws2812_position_t position,ws2812_color_t color,uint8_t sequence,uint8_t brightness)
{
  set_led(position,color,sequence,brightness);
}