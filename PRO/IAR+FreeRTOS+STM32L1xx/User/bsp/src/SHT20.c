#include "includes.h"

static uint32_t sht20_timeout;

typedef struct
{
	float temperature;
	sht20_status_t sht20_status;
}sht20_temperature_t;

typedef struct
{
	float rh;
	sht20_status_t sht20_status;
}sht20_rh_t;

static sht20_temperature_t sht20_read_temp(void)
{
	uint8_t temp_h,temp_l;
	uint16_t temp;
	sht20_temperature_t sht20_temperature;
	
	sht20_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((sht20_timeout--) == 0) 
		{
			i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}
	}
	
	I2C_Send7bitAddress(I2C1,SHT20WriteAddress,I2C_Direction_Transmitter);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}
	}
	
	I2C_Cmd(I2C1,ENABLE);
	
	I2C_SendData(I2C1,SHT20TrigTMeasureNM);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}
	}
	
	I2C_GenerateSTOP(I2C1,ENABLE);

    vTaskDelay(70 / portTICK_RATE_MS);

	I2C_GenerateSTART(I2C1,ENABLE);
	sht20_timeout = I2C1_FLAG_TIMEOUT;	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}
	}	
		
	I2C_Send7bitAddress(I2C1,SHT20ReadAddress,I2C_Direction_Receiver);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}
	}
	
	sht20_timeout = I2C1_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}
	}
	temp_h = I2C_ReceiveData(I2C1);
	
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}		
	}
	temp_l = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);

	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((sht20_timeout--) == 0)
		{
		  i2c1_recover();
			sht20_temperature.sht20_status = SHT20_ERR;
			sht20_temperature.temperature = 0;
			return sht20_temperature;
		}		
	}
	I2C_ReceiveData(I2C1);//获取crc位，但没有用到
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	I2C_AcknowledgeConfig(I2C1,ENABLE);
	
	temp_l = temp_l & 0xFC; //status位置0
	temp = ((uint16_t)(temp_h) << 8) | temp_l;
	sht20_temperature.temperature = ((float)temp / 65536) * 175.72 - 46.85;
	sht20_temperature.sht20_status = SHT20_OK;
	
	return sht20_temperature;
}

static sht20_rh_t sht20_read_rh(void)
{
	uint8_t rh_h,rh_l;
	uint16_t rh;
	sht20_rh_t sht20_rh;
	
	sht20_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	
	I2C_Send7bitAddress(I2C1,SHT20WriteAddress,I2C_Direction_Transmitter);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	
	I2C_Cmd(I2C1,ENABLE);
	
	I2C_SendData(I2C1,SHT20TrigRHMeasureNM);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	
	I2C_GenerateSTOP(I2C1,ENABLE);

    vTaskDelay(30 / portTICK_RATE_MS);

	I2C_GenerateSTART(I2C1,ENABLE);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
		
	I2C_Send7bitAddress(I2C1,SHT20ReadAddress,I2C_Direction_Receiver);
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	
	sht20_timeout = I2C1_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	rh_h = I2C_ReceiveData(I2C1);
	
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	rh_l = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);
	
	sht20_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((sht20_timeout--) == 0)
		{
			i2c1_recover();
			sht20_rh.sht20_status = SHT20_ERR;
			sht20_rh.rh = 0;
			return sht20_rh;
		}
	}
	I2C_ReceiveData(I2C1);//获取crc位，但没有用到
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	I2C_AcknowledgeConfig(I2C1,ENABLE);
	
	rh_l = rh_l & 0xFC; //status位置0
	rh = ((uint16_t)(rh_h) << 8) | rh_l;
	
	sht20_rh.rh = ((float)rh / 65536) * 125 - 6;
	sht20_rh.sht20_status = SHT20_OK;
	
	return sht20_rh;
}

sht20_result_t getTempRh(void)
{
	sht20_result_t sht20_result;
	sht20_temperature_t sht20_temperature;
	sht20_rh_t sht20_rh;
	
	sht20_temperature = sht20_read_temp();
	sht20_rh = sht20_read_rh();
	
	sht20_result.temperature = sht20_temperature.temperature;
	sht20_result.rh = sht20_rh.rh;
	
	if(sht20_temperature.sht20_status & sht20_rh.sht20_status)
	{
		sht20_result.sht20_status = SHT20_OK;
	}
	else
	{
		sht20_result.sht20_status = SHT20_ERR;
	}
	
	return sht20_result;
}

