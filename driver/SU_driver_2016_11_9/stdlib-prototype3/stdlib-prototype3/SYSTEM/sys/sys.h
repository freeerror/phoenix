#ifndef __SYS_H
#define __SYS_H	

#include "stm32l1xx.h"

#define BITBAND(addr,bitnum) ((addr & 0xF0000000)+0x2000000+((addr & 0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr,bitnum) MEM_ADDR(BITBAND(addr,bitnum))

#define GPIOA_ODR_Addr (GPIOA_BASE+0x14)
#define GPIOB_ODR_Addr (GPIOB_BASE+0x14)
#define GPIOC_ODR_Addr (GPIOC_BASE+0x14)
#define GPIOD_ODR_Addr (GPIOD_BASE+0x14)
#define GPIOE_ODR_Addr (GPIOE_BASE+0x14)
//#define GPIOF_ODR_Addr (GPIOF_BASE+14)
//#define GPIOG_ODR_Addr (GPIOG_BASE+14)

#define GPIOA_IDR_Addr (GPIOA_BASE+0x10)
#define GPIOB_IDR_Addr (GPIOB_BASE+0x10)
#define GPIOC_IDR_Addr (GPIOC_BASE+0x10)
#define GPIOD_IDR_Addr (GPIOD_BASE+0x10)
#define GPIOE_IDR_Addr (GPIOE_BASE+0x10)
//#define GPIOF_IDR_Addr (GPIOF_BASE+10)
//#define GPIOG_IDR_Addr (GPIOG_BASE+10)

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr,n)
#define PAin(n)  BIT_ADDR(GPIOA_IDR_Addr,n)
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)  BIT_ADDR(GPIOB_IDR_Addr,n)
#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr,n)
#define PCin(n)  BIT_ADDR(GPIOC_IDR_Addr,n)
#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)  BIT_ADDR(GPIOD_IDR_Addr,n)
#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)  BIT_ADDR(GPIOE_IDR_Addr,n)
//#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr,n)
//#define PFin(n)  BIT_ADDR(GPIOF_IDR_Addr,n)
//#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr,n)
//#define PGin(n)  BIT_ADDR(GPIOG_IDR_Addr,n)

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址

#endif
