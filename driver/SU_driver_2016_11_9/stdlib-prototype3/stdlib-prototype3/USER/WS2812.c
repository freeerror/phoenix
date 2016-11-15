#include "WS2812.h"
#include "delay.h"

uint16_t LED_BYTE_Buffer[LED_BIT_CNT] = {PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         };

void Timer2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD; 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	TIM2->CCMR1 |= 0x0008;
	
	DMA_DeInit(DMA1_Channel5);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)TIM2_CCR1_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)LED_BYTE_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 42;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);
	
	TIM_DMACmd(TIM2,TIM_DMA_CC1,ENABLE);
}

void setled(ws2812_position_t position,ws2812_color_t color,uint8_t sequence,uint8_t brightness)
{
	uint8_t bit_cnt;
  base_color_t base_color;
	uint8_t i;
	
	bit_cnt = position * 24;
	
	switch(color)
	{
		case black:
			base_color.red = 0;
		  base_color.green = 0;
		  base_color.blue = 0;
		  break;
		
		case red:
			base_color.red = brightness;
		  base_color.green = 0;
		  base_color.blue = 0;
		  break;  
		
		case yellow:
			base_color.red = brightness;
		  base_color.green = brightness;
		  base_color.blue = 0;
		  break;
		
		case white:
			base_color.red = brightness;
		  base_color.green = brightness;
		  base_color.blue = brightness;
			break;
		
		case purple:
			base_color.red = brightness;
		  base_color.green = 0;
		  base_color.blue = brightness;
			break;
		
		case blue:
			base_color.red = 0;
		  base_color.green = 0;
		  base_color.blue = brightness;
			break;
		
		case skyblue:
			base_color.red = 0;
		  base_color.green = brightness;
		  base_color.blue = brightness;
			break;
		
		case green:
			base_color.red = 0;
		  base_color.green = brightness;
		  base_color.blue = 0;
			break;
		
		default:
			break;
	}
	
	for(i = 0;i < 8;i++)
	{
		if((base_color.green << i) & 0x80)
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_1;
		}
		else
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_0;
		}
		bit_cnt++;
	}
	
	for(i = 0;i < 8;i++)
	{
		if((base_color.red << i) & 0x80)
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_1;
		}
		else
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_0;
		}
		bit_cnt++;
	}
	
	for(i = 0;i < 8;i++)
	{
		if((base_color.blue << i) & 0x80)
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_1;
		}
		else
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_0;
		}
		bit_cnt++;
	}
	
	DMA_SetCurrDataCounter(DMA1_Channel5,LED_BIT_CNT);
	DMA_Cmd(DMA1_Channel5,ENABLE);
  TIM_Cmd(TIM2,ENABLE);
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));
	TIM_Cmd(TIM2,DISABLE);
	DMA_Cmd(DMA1_Channel5,DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC5);
}
