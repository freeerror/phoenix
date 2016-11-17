#ifndef __WS2812_H__
#define __WS2812_H__

#include "stm32l1xx.h"
#include "sys.h"

#define TIM2_CCR1_ADDRESS  0x40000034

#define PWM_PERIOD         0x0f
#define PWM_DUTY_1         0x02
#define PWM_DUTY_0         0x01

#define LED_BIT_CNT        192

typedef enum
{
	right_right = 0,
	right_left,
	left_right,
	left_left,
	bottom_left,
	top_left,
	top_right,
	bottom_right
}ws2812_position_t;

typedef enum
{
	black   = 0,
	red     = 32,
	yellow  = 64,
	white   = 96,
	purple  = 128,
	blue    = 160,
	skyblue = 192,
	green   = 224
}ws2812_color_t;

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}base_color_t;

extern uint16_t LED_BYTE_Buffer[LED_BIT_CNT];

extern void Timer2_Init(void);

extern void setled(ws2812_position_t position,ws2812_color_t color,uint8_t sequence,uint8_t brightness);
	
#endif
