#include "opt3001.h"
#include "I2c.h"
#include "math.h"

static uint32_t opt3001_timeout;
uint8_t LowHighLimitTrigFlag;

typedef struct 
{
	uint16_t reg_dat;
	opt3001_status_t opt3001_status;
}opt3001_reg_dat_t;

static opt3001_status_t opt3001_write_two_byte(uint8_t reg_add,uint16_t dat)
{
	opt3001_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((opt3001_timeout--) == 0) 
		{
			i2c1_recover();
			return OPT3001_ERR;
		}
	}	
	
	I2C_GenerateSTART(I2C1,ENABLE);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((opt3001_timeout--) == 0)
		{
		  i2c1_recover();
			return OPT3001_ERR;
		}
	}
	
	I2C_Send7bitAddress(I2C1,OPT3001WriteAddress,I2C_Direction_Transmitter);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((opt3001_timeout--) == 0)
		{
		  i2c1_recover();
			return OPT3001_ERR;
		}
	}
	
	I2C_Cmd(I2C1,ENABLE);
	
	I2C_SendData(I2C1,reg_add);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((opt3001_timeout--) == 0)
		{
		  i2c1_recover();
			return OPT3001_ERR;
		}
	}
	
	I2C_SendData(I2C1,(uint8_t)(dat >> 8));
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((opt3001_timeout--) == 0)
		{
		  i2c1_recover();
			return OPT3001_ERR;
		}
	}	
	
	I2C_SendData(I2C1,(uint8_t)(dat));
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((opt3001_timeout--) == 0)
		{
		  i2c1_recover();
			return OPT3001_ERR;
		}
	}	
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	return OPT3001_OK;
}

static opt3001_reg_dat_t opt3001_read_two_byte(uint8_t reg_add)
{
	uint16_t temp_h,temp_l;
	opt3001_reg_dat_t opt3001_reg_dat;
	
	opt3001_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((opt3001_timeout--) == 0) 
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	}	

	I2C_GenerateSTART(I2C1,ENABLE);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((opt3001_timeout--) == 0)
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	} 

	I2C_Send7bitAddress(I2C1,OPT3001WriteAddress,I2C_Direction_Transmitter);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((opt3001_timeout--) == 0)
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	}
	
	I2C_SendData(I2C1,reg_add);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((opt3001_timeout--) == 0)
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	}	
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	I2C_GenerateSTART(I2C1,ENABLE);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((opt3001_timeout--) == 0)
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	}	
	
	I2C_Send7bitAddress(I2C1,OPT3001ReadAddress,I2C_Direction_Receiver);
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((opt3001_timeout--) == 0)
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	}

	opt3001_timeout = I2C1_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((opt3001_timeout--) == 0)
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	}
	temp_h = I2C_ReceiveData(I2C1);	
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);
	
	opt3001_timeout = I2C1_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((opt3001_timeout--) == 0)
		{
			i2c1_recover();
			opt3001_reg_dat.opt3001_status   = OPT3001_ERR;
			opt3001_reg_dat.reg_dat = 0;
			return opt3001_reg_dat;
		}
	}
	temp_l = I2C_ReceiveData(I2C1);		
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	I2C_AcknowledgeConfig(I2C1,ENABLE);

  opt3001_reg_dat.reg_dat = (temp_h << 8) | temp_l;
	opt3001_reg_dat.opt3001_status = OPT3001_OK;
	
	return opt3001_reg_dat;
}

void opt3001_int_init(void)//opt3001中断初始化
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource1);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

opt3001_status_t opt3001_reg_init(void)
{
	opt3001_config_reg_t opt3001_config_reg;
	opt3001_status_t opt3001_status;
	
	opt3001_config_reg.opt3001_config_reg_bits.RN = OPT3001AutoFullScale;
	opt3001_config_reg.opt3001_config_reg_bits.CT = OPT3001ConvertTime100ms;
	opt3001_config_reg.opt3001_config_reg_bits.M  = OPT3001ContinueConvert;
	opt3001_config_reg.opt3001_config_reg_bits.L = OPT3001InterruptLatch;
	opt3001_config_reg.opt3001_config_reg_bits.POL = OPT3001INTPinPolarityLow;
	opt3001_config_reg.opt3001_config_reg_bits.ME = OPT3001UnMSKExFiled;
	opt3001_config_reg.opt3001_config_reg_bits.FC = OPT3001FaultTriggerINTCount;	
	
	opt3001_status = opt3001_write_two_byte(OPT3001ConfigAdd,opt3001_config_reg.opt3001_config_reg_total);
		
	return opt3001_status;
}

static opt3001_result_reg_t opt3001_data_convert(float lux_limit)
{
  uint32_t lux_temp;
	opt3001_result_reg_t opt3001_result_reg;
	
	lux_temp = (uint32_t)(lux_limit * 100);
	
	if(lux_temp > 0 && lux_temp <= 4095L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 0;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp;
	}
	else if(lux_temp > 4095L && lux_temp <= 8190L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 1;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 1;		
	}
	else if(lux_temp > 8190L && lux_temp <= 16380L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 2;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 2;
	}
	else if(lux_temp > 16380L && lux_temp <= 32760L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 3;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 3;
	}
	else if(lux_temp > 32760L && lux_temp <= 65520L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 4;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 4;
	}
	else if(lux_temp > 65520L && lux_temp <= 131040L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 5;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 5;
	}
	else if(lux_temp > 131040L && lux_temp <= 262080L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 6;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 6;
	}
	else if(lux_temp > 262080L && lux_temp <= 524160L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 7;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 7;
	}
	else if(lux_temp > 524160L && lux_temp <= 1048320L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 8;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 8;
	}
	else if(lux_temp > 1048320L && lux_temp <= 2096640L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 9;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 9;
	}
	else if(lux_temp > 2096640L && lux_temp <= 4193280L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 10;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 10;
	}
	else if(lux_temp > 4193280L && lux_temp <= 8386560L)
	{
		opt3001_result_reg.opt3001_result_reg_bits.E = 11;
		opt3001_result_reg.opt3001_result_reg_bits.R = lux_temp >> 11;
	}
	
	return opt3001_result_reg;
}

opt3001_status_t opt3001_alarm_init(float opt3001_low_limit,float opt3001_high_limit)
{
	opt3001_status_t opt3001_status;
	opt3001_result_reg_t opt3001_low_limit_reg,opt3001_high_limit_reg;
	
	opt3001_low_limit_reg = opt3001_data_convert(opt3001_low_limit);
	opt3001_high_limit_reg = opt3001_data_convert(opt3001_high_limit);
	
	opt3001_status = opt3001_write_two_byte(OPT3001LowLimitAdd,opt3001_low_limit_reg.opt3001_result_reg_total);
	opt3001_status = opt3001_write_two_byte(OPT3001HighLimitAdd,opt3001_high_limit_reg.opt3001_result_reg_total);
	
	return opt3001_status;
}

opt3001_result_t opt3001_get_lux(void)
{
	opt3001_result_reg_t opt3001_result_reg;
	opt3001_reg_dat_t opt3001_reg_dat;
	opt3001_result_t opt3001_result;

	
	opt3001_reg_init();
	
  opt3001_reg_dat = opt3001_read_two_byte(OPT3001ResultAdd);
	
	opt3001_result_reg.opt3001_result_reg_total = opt3001_reg_dat.reg_dat;
	
	opt3001_result.opt3001_status = opt3001_reg_dat.opt3001_status;
	
	opt3001_result.lux = 0.01 * pow(2,opt3001_result_reg.opt3001_result_reg_bits.E) * (opt3001_result_reg.opt3001_result_reg_bits.R);
	
	return opt3001_result;
}

opt3001_result_t opt3001_int_get_lux(void)
{
	opt3001_result_reg_t opt3001_result_reg;
	opt3001_reg_dat_t opt3001_reg_dat_config,opt3001_reg_dat_result;
	opt3001_result_t opt3001_result;
	
	opt3001_reg_dat_result = opt3001_read_two_byte(OPT3001ResultAdd);
	opt3001_reg_dat_config = opt3001_read_two_byte(OPT3001ConfigAdd); //必须要读取配置寄存器，否则中断引脚电平无法清除
	
	opt3001_result_reg.opt3001_result_reg_total = opt3001_reg_dat_result.reg_dat;
	
	opt3001_result.lux = 0.01 * pow(2,opt3001_result_reg.opt3001_result_reg_bits.E) * (opt3001_result_reg.opt3001_result_reg_bits.R);	
	
	if(opt3001_reg_dat_config.opt3001_status & opt3001_reg_dat_result.opt3001_status)
	{
		opt3001_result.opt3001_status = OPT3001_OK;
	}
	else
	{
		opt3001_result.opt3001_status = OPT3001_ERR;
	}
	
	return opt3001_result;
}
