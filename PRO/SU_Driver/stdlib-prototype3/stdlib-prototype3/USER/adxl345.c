#include "ADXL345.h"
#include "I2C.h"

typedef struct
{
	uint8_t reg_dat;
	adxl345_status_t adxl345_status;
}adxl345_reg_t;

typedef struct
{
	uint8_t adxl345_result[6];
	adxl345_status_t adxl345_status;
}adxl345_regs_t;

union 
{
	uint8_t data_format_total;
	
	struct
	{
		uint8_t range        : 2;
		uint8_t jusitify     : 1;
		uint8_t full_res     : 1;
		uint8_t d4           : 1;
		uint8_t int_invert   : 1;
		uint8_t spi          : 1;
		uint8_t self_test    : 1;
	}data_format_bits;
	
}data_format_reg;

union
{
	uint8_t power_ctl_total;
	
	struct
	{
		uint8_t wakeup      : 2;
		uint8_t sleep       : 1;
		uint8_t measure     : 1;
		uint8_t auto_sleep  : 1;
		uint8_t link        : 1;
		uint8_t d6          : 1;
		uint8_t d7          : 1;
	}power_ctl_bits;
	
}power_ctl_reg;

union
{
	uint8_t bw_rate_total;
	
	struct
	{
		uint8_t rate        : 4;
		uint8_t low_power   : 1;
		uint8_t d5          : 1;
		uint8_t d6          : 1;
		uint8_t d7          : 1;
	}bw_rate_bits;
	
}bw_rate_reg;

union
{
	uint8_t act_inact_ctl_total;
	
	struct
	{
		uint8_t inact_z_en    : 1;
		uint8_t inact_y_en    : 1;
		uint8_t inact_x_en    : 1;
		uint8_t inact_ac_dc   : 1;
		uint8_t act_z_en      : 1;
		uint8_t act_y_en      : 1;
		uint8_t act_x_en      : 1;
		uint8_t act_ac_dc     : 1;
	}act_inact_ctl_bits;
	
}act_inact_ctl_reg;

typedef union
{
	uint8_t interrupt_reg_total;
	
	struct
	{
		uint8_t overrun     : 1;
		uint8_t watermark   : 1;
		uint8_t free_fall   : 1;
		uint8_t inactivity  : 1;
		uint8_t activity    : 1;
		uint8_t double_tap  : 1;
		uint8_t single_tap  : 1;
		uint8_t data_ready  : 1;
	}interrupt_reg_bits;
	
}interrupt_reg_t;

typedef struct
{
	int16_t acc_x_int;
	int16_t acc_y_int;
	int16_t acc_z_int;
}adxl345_data_int_t;

uint8_t ADXL345IntFlag = 0;

static uint32_t adxl345_timeout;
static interrupt_reg_t interrupt_reg_enable;
static interrupt_reg_t interrupt_reg_source;
static interrupt_reg_t interrupt_reg_intmap;

//STM32的I2C的ACK信号是自动产生的，通过I2C_AcknowledgeConfig(I2Cx,ENABLE)函数可以配置为每当收到一个byte发ACK信号，
//也可以通过I2C_AcknowledgeConfig(I2Cx,DISABLE)配置为每当收到一个byte发NACK信号
adxl345_reg_t adxl345_read_one_byte(uint8_t reg_add)
{
	adxl345_reg_t adxl345_reg;
	//Check busy
	adxl345_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((adxl345_timeout--) == 0) 
		{
			i2c1_recover();
			adxl345_reg.adxl345_status = ADXL345_ERR;
			adxl345_reg.reg_dat = 0;
			return adxl345_reg;
		}
	}
	//Start
	I2C_GenerateSTART(I2C1,ENABLE);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_reg.adxl345_status = ADXL345_ERR;
			adxl345_reg.reg_dat = 0;
			return adxl345_reg;
		}
	}
	//Send slave address and write bit
	I2C_Send7bitAddress(I2C1,ADXL345WriteAdd,I2C_Direction_Transmitter);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_reg.adxl345_status = ADXL345_ERR;
			adxl345_reg.reg_dat = 0;
			return adxl345_reg;
		}
	}
	
	I2C_Cmd(I2C1,ENABLE);
	//Send register address
	I2C_SendData(I2C1,reg_add);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((adxl345_timeout--) == 0)

		{
			i2c1_recover();
			adxl345_reg.adxl345_status = ADXL345_ERR;
			adxl345_reg.reg_dat = 0;
			return adxl345_reg;
		}
	}
	//Stop
	I2C_GenerateSTOP(I2C1,ENABLE);
	//Start
	I2C_GenerateSTART(I2C1,ENABLE);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_reg.adxl345_status = ADXL345_ERR;
			adxl345_reg.reg_dat = 0;
			return adxl345_reg;
		}
	}
  //Send slave address and read bits
  I2C_Send7bitAddress(I2C1,ADXL345ReadAdd,I2C_Direction_Receiver);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_reg.adxl345_status = ADXL345_ERR;
			adxl345_reg.reg_dat = 0;
			return adxl345_reg;
		}
	}	
	//Config ACK to NACK
  I2C_AcknowledgeConfig(I2C1,DISABLE);
	//Receive byte
  adxl345_timeout = I2C1_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_reg.adxl345_status = ADXL345_ERR;
			adxl345_reg.reg_dat = 0;
			return adxl345_reg;
		}
	}
	adxl345_reg.reg_dat = I2C_ReceiveData(I2C1);
	//Stop
	I2C_GenerateSTOP(I2C1,ENABLE);
	//Config NACK to ACK
	I2C_AcknowledgeConfig(I2C1,ENABLE);
	
	adxl345_reg.adxl345_status = ADXL345_OK;
	
	return adxl345_reg;
}

adxl345_status_t adxl345_write_one_byte(uint8_t reg_add,uint8_t dat)
{
	adxl345_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((adxl345_timeout--) == 0) 
		{
			i2c1_recover();
			return ADXL345_ERR;
		}
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((adxl345_timeout--) == 0)
		{
		  i2c1_recover();
			return ADXL345_ERR;
		}
	}

	I2C_Send7bitAddress(I2C1,ADXL345WriteAdd,I2C_Direction_Transmitter);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((adxl345_timeout--) == 0)
		{
		  i2c1_recover();
			return ADXL345_ERR;
		}
	}
	
	I2C_Cmd(I2C1,ENABLE);	
	
	I2C_SendData(I2C1,reg_add);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((adxl345_timeout--) == 0)
		{
		  i2c1_recover();
			return ADXL345_ERR;
		}
	}
	
	I2C_SendData(I2C1,dat);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((adxl345_timeout--) == 0)
		{
		  i2c1_recover();
			return ADXL345_ERR;
		}
	}
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	return ADXL345_OK;
}

adxl345_regs_t adxl345_read_bytes(uint8_t reg_add,uint8_t len)
{
	adxl345_regs_t adxl345_regs;
	uint8_t i;
	
	adxl345_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((adxl345_timeout--) == 0) 
		{
			i2c1_recover();
			adxl345_regs.adxl345_status = ADXL345_ERR;
			return adxl345_regs;
		}
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_regs.adxl345_status = ADXL345_ERR;
			return adxl345_regs;
		}
	}	
	
	I2C_Send7bitAddress(I2C1,ADXL345WriteAdd,I2C_Direction_Transmitter);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_regs.adxl345_status = ADXL345_ERR;
			return adxl345_regs;
		}
	}
	
	I2C_Cmd(I2C1,ENABLE);
	
	I2C_SendData(I2C1,reg_add);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((adxl345_timeout--) == 0)

		{
			i2c1_recover();
			adxl345_regs.adxl345_status = ADXL345_ERR;
			return adxl345_regs;
		}
	}
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	I2C_GenerateSTART(I2C1,ENABLE);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_regs.adxl345_status = ADXL345_ERR;
			return adxl345_regs;
		}
	}
	
  I2C_Send7bitAddress(I2C1,ADXL345ReadAdd,I2C_Direction_Receiver);
	adxl345_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_regs.adxl345_status = ADXL345_ERR;
			return adxl345_regs;
		}
	}
	
	for(i = 0;i < len - 1;i++)
	{
		  adxl345_timeout = I2C1_FLAG_TIMEOUT;
			while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
			{
				if((adxl345_timeout--) == 0)
				{
					i2c1_recover();
					adxl345_regs.adxl345_status = ADXL345_ERR;
					return adxl345_regs;
				}
			}
			adxl345_regs.adxl345_result[i] = I2C_ReceiveData(I2C1);
	}
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);
	
  adxl345_timeout = I2C1_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((adxl345_timeout--) == 0)
		{
			i2c1_recover();
			adxl345_regs.adxl345_status = ADXL345_ERR;
			return adxl345_regs;
		}
	}
	adxl345_regs.adxl345_result[i] = I2C_ReceiveData(I2C1);
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	I2C_AcknowledgeConfig(I2C1,ENABLE);
	
	adxl345_regs.adxl345_status = ADXL345_OK;
	
	return adxl345_regs;	
}

//adxl345_status_t adxl345_init(void)
//{
//	adxl345_status_t adxl345_status;
//	
//	//Stop measure
//	power_ctl_reg.power_ctl_bits.measure = ADXL345StopMeasure;
//	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
//	if(adxl345_status == ADXL345_ERR)
//	{
//		return ADXL345_ERR;
//	}
//	//Config data format
//	data_format_reg.data_format_bits.full_res = ADXL345FullResolution;
//	data_format_reg.data_format_bits.range = ADXL345Range16g;
//	adxl345_status = adxl345_write_one_byte(ADXL345DataFormatRegAdd,data_format_reg.data_format_total);
//	if(adxl345_status == ADXL345_ERR)
//	{
//		return ADXL345_ERR;
//	}
//	//Config BW_RATE
//	bw_rate_reg.bw_rate_bits.low_power = ADXL345NormalOper;
//	bw_rate_reg.bw_rate_bits.rate = ADXL345BWRate100Hz;
//	adxl345_status = adxl345_write_one_byte(ADXL345BWRateAdd,bw_rate_reg.bw_rate_total);
//	if(adxl345_status == ADXL345_ERR)
//	{
//		return ADXL345_ERR;
//	}
//	//Config interrupt pin
//	interrupt_reg_map.interrupt_reg_bits.data_ready = ADXL345IntMapPin1;
//  adxl345_status = adxl345_write_one_byte(ADXL345IntMapAdd,interrupt_map_reg.interrupt_reg_total);
//	if(adxl345_status == ADXL345_ERR)
//	{
//		return ADXL345_ERR;
//	}
//	//Config interrupt
//	interrupt_enable_reg.interrupt_reg_bits.data_ready = ADXL345DataRdyIntEn;
//	adxl345_status = adxl345_write_one_byte(ADXL345IntEnableAdd,interrupt_enable_reg.interrupt_reg_total);
//	if(adxl345_status == ADXL345_ERR)
//	{
//		return ADXL345_ERR;
//	}
//	
//	power_ctl_reg.power_ctl_bits.measure = ADXL345StartMeasure;
//	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
//	if(adxl345_status == ADXL345_ERR)
//	{
//		return ADXL345_ERR;
//	}
//	
//	return adxl345_status;
//}

adxl345_result_t getAcceleration(void)
{
	adxl345_regs_t adxl345_regs;
	adxl345_result_t adxl345_result;
	adxl345_data_int_t adxl345_data_int;
	
	adxl345_regs = adxl345_read_bytes(ADXL345DATAX0RegAdd,6);
	
  adxl345_data_int.acc_x_int = ((int16_t)adxl345_regs.adxl345_result[1] << 8) | adxl345_regs.adxl345_result[0];
	adxl345_data_int.acc_y_int = ((int16_t)adxl345_regs.adxl345_result[3] << 8) | adxl345_regs.adxl345_result[2];
	adxl345_data_int.acc_z_int = ((int16_t)adxl345_regs.adxl345_result[5] << 8) | adxl345_regs.adxl345_result[4];
	
	adxl345_result.x = ((float)adxl345_data_int.acc_x_int) * ADXL345ScaleFactor /((float)1000);
	adxl345_result.y = ((float)adxl345_data_int.acc_y_int) * ADXL345ScaleFactor /((float)1000);
	adxl345_result.z = ((float)adxl345_data_int.acc_z_int) * ADXL345ScaleFactor /((float)1000);
	
	adxl345_result.adxl345_status = adxl345_regs.adxl345_status;

	return adxl345_result;
}

void adxl345_low_level_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

#ifdef ADXL345IntPinOnPB2 //prototype3的硬件中ADXL345的中断引脚连接在PB2脚	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource2);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);	
	
#endif

#ifdef ADXL345IntPinOnPC15 //prototype2的硬件中ADXL345的中断引脚连接在PC15

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource15);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line15;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);	

#endif
	
	interrupt_reg_intmap.interrupt_reg_total = 0;
	interrupt_reg_source.interrupt_reg_total = 0;
	interrupt_reg_enable.interrupt_reg_total = 0;
}

adxl345_status_t adxl345_freefallint_disable(void)
{
	adxl345_status_t adxl345_status;
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StopMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
	if( adxl345_status == ADXL345_ERR )
	{
		return ADXL345_ERR;
	}
	
	interrupt_reg_enable.interrupt_reg_bits.free_fall = ADXL345FreefallIntDis;
	adxl345_status = adxl345_write_one_byte(ADXL345IntEnableAdd,interrupt_reg_enable.interrupt_reg_total);
	if( adxl345_status == ADXL345_ERR )
	{
		return ADXL345_ERR;
	}
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StartMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);	
  if( adxl345_status == ADXL345_ERR )
	{
		return ADXL345_ERR;
	}
	
	return adxl345_status;	
}

adxl345_status_t adxl345_actint_disable(void)
{
	adxl345_status_t adxl345_status;
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StopMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
	if( adxl345_status == ADXL345_ERR )
	{
		return ADXL345_ERR;
	}
	
	interrupt_reg_enable.interrupt_reg_bits.activity = ADXL345ActIntDis;
	adxl345_status = adxl345_write_one_byte(ADXL345IntEnableAdd,interrupt_reg_enable.interrupt_reg_total);
	if( adxl345_status == ADXL345_ERR )
	{
		return ADXL345_ERR;
	}
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StartMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);	
  if( adxl345_status == ADXL345_ERR )
	{
		return ADXL345_ERR;
	}
	
	return adxl345_status;
}

adxl345_status_t observeActivity(int8_t thresh , bool_t x, bool_t y , bool_t z)
{
	adxl345_status_t adxl345_status;
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StopMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}	
	
  adxl345_status = adxl345_write_one_byte(ADXL345ThreshActAdd,thresh);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}	
	
	act_inact_ctl_reg.act_inact_ctl_bits.act_ac_dc = ADXL345ActDCEn;
	act_inact_ctl_reg.act_inact_ctl_bits.act_x_en = x;
	act_inact_ctl_reg.act_inact_ctl_bits.act_y_en = y;
	act_inact_ctl_reg.act_inact_ctl_bits.act_z_en = z;
	adxl345_status = adxl345_write_one_byte(ADXL345ActInactCtlAdd,act_inact_ctl_reg.act_inact_ctl_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}	
	
	interrupt_reg_intmap.interrupt_reg_bits.activity = ADXL345IntMapPin1;
	adxl345_status = adxl345_write_one_byte(ADXL345IntMapAdd,interrupt_reg_intmap.interrupt_reg_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}
	
	interrupt_reg_enable.interrupt_reg_bits.activity = ADXL345ActIntEn;
	adxl345_status = adxl345_write_one_byte(ADXL345IntEnableAdd,interrupt_reg_enable.interrupt_reg_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}	
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StartMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}
	
	return adxl345_status;
}

adxl345_result_t adxl345_act_freefall_int_process(void)
{
	adxl345_status_t adxl345_status;
	adxl345_reg_t adxl345_reg;
	adxl345_result_t adxl345_result;
	
	if(ADXL345IntFlag)
	{
		ADXL345IntFlag = 0;//中断标志清零
		
		adxl345_reg = adxl345_read_one_byte(ADXL345ActTapStatus);//清中断前先读取ACT_TAP_STATUS寄存器
		if(adxl345_reg.adxl345_status == ADXL345_ERR)
		{
			adxl345_result.adxl345_status = ADXL345_ERR;
			return adxl345_result;
		}
		
		adxl345_reg = adxl345_read_one_byte(ADXL345IntSourceAdd);//读取中断源，并清除中断
		if(adxl345_reg.adxl345_status == ADXL345_ERR)
		{
			adxl345_result.adxl345_status = ADXL345_ERR;
			return adxl345_result;
		}
		
		interrupt_reg_source.interrupt_reg_total = adxl345_reg.reg_dat;//读取的中断源数据赋值给变量
		
    if(interrupt_reg_source.interrupt_reg_bits.activity)//判断是否activity中断
		{
			adxl345_status = adxl345_actint_disable();//Activity中断失能
			if(adxl345_status == ADXL345_ERR)
			{
				adxl345_result.adxl345_status = ADXL345_ERR;
				return adxl345_result;
			}
			adxl345_result = getAcceleration();
			return adxl345_result;
		}
    else if(interrupt_reg_source.interrupt_reg_bits.free_fall)//判断是否freefall中断
		{
			adxl345_status = adxl345_freefallint_disable();//freefall中断失能
			if(adxl345_status == ADXL345_ERR)
			{
				adxl345_result.adxl345_status = ADXL345_ERR;
				return adxl345_result;
			}
			adxl345_result = getAcceleration();
			return adxl345_result;			
		}
		else
		{
			return adxl345_result;
		}
	}
	else
	{
		return adxl345_result;		
	}
}

//adxl345_result_t adxl345_actint_process(void)
//{
//	adxl345_status_t adxl345_status;
//	adxl345_reg_t adxl345_reg;
//	adxl345_result_t adxl345_result;
//	
//	if(ADXL345IntFlag)
//	{
//		ADXL345IntFlag = 0;//中断标志清零
//		
//		adxl345_status = adxl345_actint_disable();//Activity中断失能
//		if(adxl345_status == ADXL345_ERR)
//		{
//			adxl345_result.adxl345_status = ADXL345_ERR;
//			return adxl345_result;
//		}
//		
//		adxl345_reg = adxl345_read_one_byte(ADXL345IntSourceAdd);//读取中断源，并清除中断
//		if(adxl345_reg.adxl345_status == ADXL345_ERR)
//		{
//			adxl345_result.adxl345_status = ADXL345_ERR;
//			return adxl345_result;			
//		}
//		
//		interrupt_reg_source.interrupt_reg_total = adxl345_reg.reg_dat;//读取的中断源数据赋值给变量
//		
//		if(interrupt_reg_source.interrupt_reg_bits.activity)//判断是否activity中断
//		{
//			adxl345_result = getAcceleration();
//			return adxl345_result;
//		}
//		else
//		{
//			adxl345_result.adxl345_status = ADXL345_ERR;
//			return adxl345_result;
//		}
//	}
//	else
//	{
//		return adxl345_result;		
//	}
//}

adxl345_status_t observeFreefall(void)
{
	adxl345_status_t adxl345_status;
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StopMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}	

	adxl345_status = adxl345_write_one_byte(ADXL345ThreshFFAdd,ADXL345ThreshFF);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}	

  adxl345_status = adxl345_write_one_byte(ADXL345TimeFFAdd,ADXL345TimeFF);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}		

	interrupt_reg_intmap.interrupt_reg_bits.free_fall = ADXL345IntMapPin1;
	adxl345_status = adxl345_write_one_byte(ADXL345IntMapAdd,interrupt_reg_intmap.interrupt_reg_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}
	
	interrupt_reg_enable.interrupt_reg_bits.free_fall = ADXL345FFIntEn;
	adxl345_status = adxl345_write_one_byte(ADXL345IntEnableAdd,interrupt_reg_enable.interrupt_reg_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}	
	
	power_ctl_reg.power_ctl_bits.measure = ADXL345StartMeasure;
	adxl345_status = adxl345_write_one_byte(ADXL345PowerCTLAdd,power_ctl_reg.power_ctl_total);
	if(adxl345_status == ADXL345_ERR)
	{
    return ADXL345_ERR;
	}
	
  return adxl345_status;	
}
