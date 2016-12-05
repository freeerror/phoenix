#ifndef __DISMANTLE_H__
#define __DISMANTLE_H__

#ifdef DEBUG_SYSTEM

    #define RCC_DISMANTLE        RCC_AHBPeriph_GPIOC
    #define GPIO_PORT_DISMANTLE  GPIOC
    #define GPIO_PIN_DISMANTLE   GPIO_Pin_13

    #define EXTI_LINE_DISMANTLE        EXTI_Line13
    #define EXTI_PortSource_DISMANTLE  EXTI_PortSourceGPIOC
    #define EXTI_PinSource_DISMANTLE   EXTI_PinSource13

    #define EXTI_IRQn_DISMANTLE        EXTI15_10_IRQn

#else       

    #define RCC_DISMANTLE        RCC_AHBPeriph_GPIOC
    #define GPIO_PORT_DISMANTLE  GPIOC
    #define GPIO_PIN_DISMANTLE   GPIO_Pin_4

    #define EXTI_LINE_DISMANTLE        EXTI_Line4
    #define EXTI_PortSource_DISMANTLE  EXTI_PortSourceGPIOC
    #define EXTI_PinSource_DISMANTLE   EXTI_PinSource4

    #define EXTI_IRQn_DISMANTLE        EXTI4_IRQn

#endif   

void dismantle_int_init(void);

bool app_check_dismantle_key_status(void);

#endif