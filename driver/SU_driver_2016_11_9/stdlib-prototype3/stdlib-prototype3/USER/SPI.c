#include "spi.h"

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);	
	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex ;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB ;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStructure);
	
	SPI_Cmd(SPI2,ENABLE);
}

//uint8_t spi2_read_write_byte(uint8_t dat)
//{
//	uint8_t retry = 0;
//	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET)
//	{
//		retry++;
//		if(retry > 200) return 0;
//	}
//	SPI_I2S_SendData(SPI2,dat);
//	return 1;
//}

//uint8_t spi2_read_write_byte(void)
//{
//	uint8_t retry = 0;
//	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		retry++;
//		if(retry > 200) return 0;
//	}		
//	return SPI_I2S_ReceiveData(SPI2);
//}

uint8_t spi2_read_write_byte(uint8_t tx_data)
{
	uint8_t retry = 0;
	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry > 200) return 0;
	}
	SPI_I2S_SendData(SPI2,tx_data);
	
	retry = 0;
	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry > 200) return 0;
	}		
	
	return SPI_I2S_ReceiveData(SPI2);
}
