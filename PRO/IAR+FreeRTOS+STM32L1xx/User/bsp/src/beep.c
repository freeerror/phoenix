#include "includes.h"

#ifdef DEBUG_SYSTEM

    #define RCC_BEEP       RCC_AHBPeriph_GPIOA
    #define GPIO_PORT_BEEP GPIOA
    #define GPIO_PIN_BEEP  GPIO_Pin_4

#else

    #define RCC_BEEP       RCC_AHBPeriph_GPIOC
    #define GPIO_PORT_BEEP GPIOC
    #define GPIO_PIN_BEEP  GPIO_Pin_9

#endif

/*
*********************************************************************************************************
*	函 数 名:beep_init(void)
*	功能说明:初始化蜂鸣器控制引脚 
*	形    参:无  
*	返 回 值:无 
*********************************************************************************************************
*/
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_BEEP,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BEEP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIO_PORT_BEEP,&GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名:beep(FunctionalState NewState)
*	功能说明:蜂鸣器蜂鸣或停止蜂鸣
*	形    参:NewState:ENABLE,蜂鸣；DISABLE,停止蜂鸣  
*	返 回 值:无 
*********************************************************************************************************
*/
void beep(FunctionalState NewState)
{
	  if (NewState != DISABLE)
	  {
	  	GPIO_PORT_BEEP->BSRRL = GPIO_PIN_BEEP;
	  }
	  else
	  {
	  	GPIO_PORT_BEEP->BRR = GPIO_PIN_BEEP;
	  }
}






