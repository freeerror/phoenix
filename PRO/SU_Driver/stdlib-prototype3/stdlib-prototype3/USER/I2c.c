#include "I2C.h"

void i2c1_init(void)
{
	GPIO_InitTypeDef GPIOInitStructure;
	I2C_InitTypeDef I2CInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);
	
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIOInitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	I2C_DeInit(I2C1);
	
	I2CInitStructure.I2C_Mode = I2C_Mode_I2C;
	I2CInitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2CInitStructure.I2C_OwnAddress1 = 0x00;
	I2CInitStructure.I2C_Ack = I2C_Ack_Enable;
	I2CInitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2CInitStructure.I2C_ClockSpeed = 100000;
	
	//I2C_ITConfig(I2C1,I2C_IT_ERR,ENABLE);
	
	I2C_Cmd(I2C1,ENABLE);
	I2C_Init(I2C1,&I2CInitStructure);
	I2C_AcknowledgeConfig(I2C1,ENABLE);
}

void i2c1_recover(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2CInitStructure;
	
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	 
	GPIO_SetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9);
	 
	I2C1->CR1 |= 0x8000;
	I2C1->CR1 &= 0x7FFF;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);
	
	I2CInitStructure.I2C_Mode = I2C_Mode_I2C;
	I2CInitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2CInitStructure.I2C_OwnAddress1 = 0x00;
	I2CInitStructure.I2C_Ack = I2C_Ack_Enable;
	I2CInitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2CInitStructure.I2C_ClockSpeed = 100000;
	
	I2C_Cmd(I2C1,ENABLE);
	I2C_Init(I2C1,&I2CInitStructure);
	I2C_AcknowledgeConfig(I2C1,ENABLE);
}
