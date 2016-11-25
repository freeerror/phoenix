#ifndef __WS2812_H__
#define __WS2812_H__

#define TIM2_CCR1_ADDRESS  0x40000034
#define TIM3_CCR3_ADDRESS  0x4000043C


#define PWM_PERIOD         0x0D
#define PWM_DUTY_1         0x02
#define PWM_DUTY_0         0x01

#define LED_BIT_CNT        (192+48)

//LEDµÆÎ»ÖÃ·Ö²¼£º
//top_left                 top_middle               top_right      
//LED103 ----------------> LED104 ----------------> LED105
//  ^
//  |
//  |
//LED102 <----------------------------------------- LED101 <--- Led_Data
//bottom_left 
#if 0
typedef enum
{
	bottom_right = 0,
	bottom_left,
	top_left,
	top_middle,
	top_right
          
}ws2812_position_t;
#else
typedef enum
{
    right_right = 0,
    right_left = 1,
    left_right = 2,
    left_left = 3,
    bottom_left = 4,
    top_left = 5,
    top_right = 6,
    bottom_right = 7,
    top_middle
}ws2812_position_t;
#endif


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

extern void WS2812_Timer_Init(void);
extern void set_led(ws2812_position_t ws2812_position,ws2812_color_t color,uint8_t sequence,uint8_t brightness);
	
#endif
