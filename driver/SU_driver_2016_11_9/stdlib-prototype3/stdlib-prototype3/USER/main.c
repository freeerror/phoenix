#include <string.h>
#include <stdio.h>
#include "stm32l1xx.h"
#include "delay.h"
#include "usart.h"
#include "GPIO.h"
#include "SX1276.h"
#include "SPI.h"
#include "WS2812.h"
#include "I2C.h"
#include "SHT20.h"
#include "opt3001.h"
#include "adxl345.h"
#include "nt3h1101.h"

//uint8_t _SYSCLKSource;
//RCC_ClocksTypeDef RCC_Clocks;

int main(void)
{	
//	sht20_result_t sht20_result;
//	opt3001_result_t opt3001_result;
//	adxl345_result_t adxl345_result;
//	nt3h1101_result_t nt3h1101_result;
//	uint8_t block_bytes[16];
//	nt3h1101_id_t nt3h1101_id;.
//  nt3h1101_reg_t nt3h1101_reg;
    uint8_t ucValue;	
//	const char *string1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!\r\n";
//	const char *string2 = "12345678901234567890123!\r\n";
//	char str[200];
	
	delay_init();
	gpio_init();
	bsp_InitUart();
	SPI2_Init();
	i2c1_init();
	Timer2_Init();
	opt3001_reg_init();
	opt3001_int_init();
	opt3001_alarm_init(100.1,1000.1);
	adxl345_low_level_init();
  observeActivity(0x3F,TRUE,TRUE,TRUE);
	observeFreefall();
	nt3h1101_init();
	nt3h1101_power_on();//每次重新上电nt3h1101配置寄存器的值就会被重新覆盖为默认值
	nt3h1101_write_reg(0x00,0x3C,0x34);//配置FD引脚
//  _SYSCLKSource = RCC_GetSYSCLKSource();
//	RCC_GetClocksFreq(&RCC_Clocks);
  delay_ms(10);

	//memset(block_bytes,0x25,16 * sizeof(block_bytes));
	//nt3h1101_write_one_block(2,block_bytes);
	//nt3h1101_result = nt3h1101_read_one_block(2);
	
  while(1)
  {
		if(comGetChar(COM2,&ucValue))
		{
			comSendChar(COM2,ucValue);
		}

//			comSendBuf(COM2,(uint8_t *)(string1),strlen(string1));
//		  delay_ms(10);
//			comSendBuf(COM2,(uint8_t *)(string2),strlen(string2));
		  delay_ms(1000);
  }
}

//		printf("请输入value，回车结束\r\n");
//		scanf("%d",&ucValue);
//		printf("所输入的整数为：value = %d\r\n",ucValue);
//		if(comGetChar(COM2,&ucValue))
//		{
//			comSendChar(COM2,ucValue);
//		}
		//nt3h1101_read_reg(0x00);
		//USART_SendData(USART2, 0x30);
		//comSendChar(COM2, 0x31);
		//comSendBuf(COM2,(uint8_t *)(string),strlen(string));
	  //delay_ms(1000);

//		printf("请输入1个字符串，回车结束\r\n");
//		scanf("%s",str);
//		printf("所输入的字符串为：str = %s\r\n",str);
//		printf("\r\n");

//    nt3h1101_reg = nt3h1101_read_reg(0x02);

//    nt3h1101_result = nt3h1101_read_one_block(1);

//		nt3h1101_id = nt3h1101_get_id();
//	  delay_ms(10);

//		adxl345_result = adxl345_act_freefall_int_process();
//		if(adxl345_result.adxl345_status == ADXL345_OK)
//		{
//			printf("x : %.3f , y : %.3f , z : %.3f\r\n",adxl345_result.x,adxl345_result.y,adxl345_result.z);
//		}

//    adxl345_result = getAcceleration();
//		if(adxl345_result.adxl345_status)
//		{
//		  printf(" x : %.2f  y : %.2f  z : %.2f \r\n",adxl345_result.x,adxl345_result.y,adxl345_result.z);
//		}
//		else
//		{
//			printf("ERROR \r\n");
//		}
//		delay_ms(1000);

//		if(ADXL345IntFlag)
//		{
//			ADXL345IntFlag = 0;
//			printf("Activity\r\n");
//			adxl345_int_clr();
//		}

//    adxl345_result = getAcceleration();
//		if(adxl345_result.adxl345_status)
//		{
//		  printf(" x : %.2f  y : %.2f  z : %.2f \r\n",adxl345_result.x,adxl345_result.y,adxl345_result.z);
//		}
//		else
//		{
//			printf("ERROR \r\n");
//		}
//		delay_ms(1000);
		
//    sht20_result = getTempRh();
//		if(sht20_result.sht20_status)
//		{
//			printf("%.2f  %.2f\r\n",sht20_result.temperature,sht20_result.rh);
//		}
//		else
//		{
//			printf("ERROR\r\n");
//		}
//		delay_ms(1000);
//		
//    opt3001_result = opt3001_get_lux();
//		if(opt3001_result.opt3001_status)
//		{
//			printf("%.2f \r\n",opt3001_result.lux);
//		}
//		else
//		{
//			printf("OPT3001_ERR\r\n");
//		}		
//		delay_ms(1000);

//    sht20_result = getTempRh();
//		if(sht20_result.sht20_status)
//		{
//			printf("%.2f  %.2f\r\n",sht20_result.temperature,sht20_result.rh);
//		}
//		else
//		{
//			printf("ERROR\r\n");
//		}


//    opt3001_result = opt3001_get_lux();
//		if(opt3001_result.opt3001_status)
//		{
//			printf("%.2f \r\n",opt3001_result.lux);
//		}
//		else
//		{
//			printf("OPT3001_ERR\r\n");
//		}

//    if(LowHighLimitTrigFlag)
//		{
//			LowHighLimitTrigFlag = 0;
//			opt3001_result = opt3001_int_get_lux();
//			if(opt3001_result.opt3001_status)
//			{
//				printf("%.2f\r\n",opt3001_result.lux);
//			}
//			else
//			{
//				printf("OPT3001_ERR\r\n");
//			}
//		}
