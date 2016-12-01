#include "includes.h"

#define RCC_DISMANTLE        RCC_AHBPeriph_GPIOC
#define GPIO_PORT_DISMANTLE  GPIOC
#define GPIO_PIN_DISMANTLE   GPIO_Pin_4

#define EXTI_LINE_DISMANTLE        EXTI_Line4
#define EXTI_PortSource_DISMANTLE  EXTI_PortSourceGPIOC
#define EXTI_PinSource_DISMANTLE   EXTI_PinSource4

#define EXTI_IRQn_DISMANTLE        EXTI4_IRQn

dismantle_trig_edge_t dismantle_trig_edge = none_edge;


void dismantle_int_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_DISMANTLE,ENABLE);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_PIN_DISMANTLE;
    GPIO_Init(GPIO_PORT_DISMANTLE,&GPIO_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSource_DISMANTLE,EXTI_PinSource_DISMANTLE);
    EXTI_InitStruct.EXTI_Line = EXTI_LINE_DISMANTLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI_IRQn_DISMANTLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

