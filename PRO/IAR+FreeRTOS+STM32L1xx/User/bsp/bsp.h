#ifndef _BSP_H_
#define _BSP_H


/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

#define DEBUG_SYSTEM

#define INT_ALARM_LEN   10
#define INT_ALARM_SIZE  1
extern  QueueHandle_t int_alarm_queue;


typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum
{
    acc_alarm = 0x01,
    brightness_alarm = 0x02,
    nfc_alarm = 0x04,
    fall_off_alarm = 0x08
}int_alarm_t;



#include "stm32l1xx.h"
#include "stm32l1xx_it.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "delay.h"
#include "i2c.h"
#include "beep.h"
#include "platform.h"
#include "opt3001.h"
#include "SHT20.h"
#include "ADXL345.h"
#include "nt3h1101.h"
#include "WS2812.h"
#include "usart.h"
#include "eeprom.h"
#include "adc.h"
#include "dismantle.h"
#include "hal_beep.h"
#include "hal_platform.h"
#include "hal_brightness.h"
#include "hal_tempRh.h"
#include "hal_acc_sensor.h"
#include "hal_nfc.h"
#include "hal_led.h"
#include "nfc_module.h"
#include "hal_energy.h"
#include "hal_dismantle.h"
#include "hal_eeprom.h"

void bsp_Init(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
