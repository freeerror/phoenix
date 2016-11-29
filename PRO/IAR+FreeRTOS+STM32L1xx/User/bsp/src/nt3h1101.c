#include "includes.h"

#define  RCC_NFC_POWER      RCC_AHBPeriph_GPIOB
#define  GPIO_PORT_NFCPWR   GPIOB
#define  GPIO_PIN_NFCPWR    GPIO_Pin_12

#define  RCC_NFC_INT        RCC_AHBPeriph_GPIOD
#define  RCC_PORT_NFCINT    GPIOD
#define  GPIO_PIN_NFCINT    GPIO_Pin_4

#define  EXTI_PORT_NFC      EXTI_PortSourceGPIOD
#define  EXTI_PinSourceNFC  EXTI_PinSource4
#define  EXTI_LINE_NFC      EXTI_Line4
#define  EXTI_IRQ_NFC       EXTI4_IRQn
#define  EXTI_NFC_PreemptionPriority   5
#define  EXTI_NFC_SubPriority          0

#define MEMA                    0xFE
#define REGA_NC_REG             0x00
#define REGA_LAST_NDEF_BLOCK    0x01
#define REGA_SRAM_MIRROR_BLOCK  0x02
#define REGA_WDT_LS             0x03
#define REGA_WDT_MS             0x04
#define REGA_I2C_CLOCK_STR      0x05
#define REGA_REG_LOCK           0x06
#define REGA_FD_MASK            0x3C
#define REG_DATA_FD             0x34 //仅当第一次成功的RF通讯以后才产生中断电平（低电平），当RF场消失后，中断电平消失（恢复到高电平）
                                     //这种情况下，当检测到RF场以后，还是会产生多次中断，但每次通过RF成功读取数据后，仅产生一次中断

nt3h1101_status_t nt3h1101_write_reg(uint8_t rega,uint8_t mask,uint8_t reg_data);

static uint32_t nt3h1101_timeout;

void nt3h1101_power_ctl_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_AHBPeriphClockCmd(RCC_NFC_POWER,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_NFCPWR;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIO_PORT_NFCPWR,&GPIO_InitStructure);	
	
	//GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void nt3h1101_power_on(void)
{
    nt3h1101_result_t nt3h1101_result;
    uint8_t retry = 0;

	GPIO_PORT_NFCPWR->BSRRL = GPIO_PIN_NFCPWR; //上电

    do
    {
        nt3h1101_result = nt3h1101_read_one_block(0x00);
        retry++;
        if(retry > 10)//多次尝试失败，则退出
        {
            printf("NT3H1101 ERROR!\r\n");
            break;
        }
    }while(nt3h1101_result.nt3h1101_status == NT3H1101_ERR);//等待NT3H1101成功回应
    
    nt3h1101_write_reg(REGA_NC_REG,REGA_FD_MASK,REG_DATA_FD);//配置FD中断引脚
}

void nt3h1101_power_off(void)
{
	GPIO_PORT_NFCPWR->BSRRH = GPIO_PIN_NFCPWR;
}

nt3h1101_result_t nt3h1101_read_one_block(uint8_t block_add)
{
	nt3h1101_result_t nt3h1101_result;
	uint8_t i;
	
	nt3h1101_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))	
	{
		if((nt3h1101_timeout--) == 0) 
		{
			i2c1_recover();
      nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_result;
		}		
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_result;
		}
	}	
	
	I2C_Send7bitAddress(I2C1,NT3H1101WriteAdd,I2C_Direction_Transmitter);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_result;
		}
	}	
	
	I2C_Cmd(I2C1,ENABLE);	

	I2C_SendData(I2C1,block_add);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_result;
		}
	}	
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	I2C_GenerateSTART(I2C1,ENABLE);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_result;
		}
	}	
	
  I2C_Send7bitAddress(I2C1,NT3H1101ReadAdd,I2C_Direction_Receiver);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_result;
		}
	}
	
	for(i = 0;i < NT3H1101OneBlockBytes;i++)
	{
		nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	  {
			if((nt3h1101_timeout--) == 0)
			{
				i2c1_recover();
				nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
				return nt3h1101_result;
			}
		}
		nt3h1101_result.block_bytes[i] = I2C_ReceiveData(I2C1);
	}

	I2C_GenerateSTOP(I2C1,ENABLE);
	
	nt3h1101_result.nt3h1101_status = NT3H1101_OK;
	
	return nt3h1101_result;
}

nt3h1101_status_t nt3h1101_write_one_block(uint8_t block_add,uint8_t block_bytes[NT3H1101OneBlockBytes])
{
	uint8_t i;
	
	nt3h1101_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	{
		if((nt3h1101_timeout--) == 0) 
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}	

	I2C_Send7bitAddress(I2C1,NT3H1101WriteAdd,I2C_Direction_Transmitter);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}	

	I2C_Cmd(I2C1,ENABLE);
	
	I2C_SendData(I2C1,block_add);
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}

  for(i = 0;i < NT3H1101OneBlockBytes;i++)
	{
		I2C_SendData(I2C1,block_bytes[i]);
		nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((nt3h1101_timeout--) == 0)
			{
				i2c1_recover();
				return NT3H1101_ERR;
			}
		}		
	}
	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	return NT3H1101_OK;	
}

nt3h1101_id_t nt3h1101_get_id(void)
{
	nt3h1101_result_t nt3h1101_result;
	nt3h1101_id_t nt3h1101_id;

	nt3h1101_result = hal_nfc_read_one_block(0x00);
	
	nt3h1101_id.nt3h1101_status = nt3h1101_result.nt3h1101_status;
	
	nt3h1101_id.id_bytes[0] = nt3h1101_result.block_bytes[0];
	nt3h1101_id.id_bytes[1] = nt3h1101_result.block_bytes[1];
	nt3h1101_id.id_bytes[2] = nt3h1101_result.block_bytes[2];
	nt3h1101_id.id_bytes[3] = nt3h1101_result.block_bytes[3];
	nt3h1101_id.id_bytes[4] = nt3h1101_result.block_bytes[4];
	nt3h1101_id.id_bytes[5] = nt3h1101_result.block_bytes[5];
	nt3h1101_id.id_bytes[6] = nt3h1101_result.block_bytes[6];
	
	return nt3h1101_id;
}

nt3h1101_reg_t nt3h1101_read_reg(uint8_t rega)
{
	nt3h1101_reg_t nt3h1101_reg;
	
	nt3h1101_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))//判断总线是否忙	
	{
		if((nt3h1101_timeout--) == 0) 
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}		
	}
	
	I2C_GenerateSTART(I2C1,ENABLE);//产生起始信号
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}
	}	
	
	I2C_Send7bitAddress(I2C1,NT3H1101WriteAdd,I2C_Direction_Transmitter);//发送从机地址写
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}
	}	
	
	I2C_Cmd(I2C1,ENABLE);	

	I2C_SendData(I2C1,MEMA);//发送MEMA地址
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}
	}	
	
	I2C_SendData(I2C1,rega);//发送REGA地址
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}
	}	
	
	I2C_GenerateSTOP(I2C1,ENABLE);//产生stop信号
	
	I2C_GenerateSTART(I2C1,ENABLE);//产生start信号
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}
	}	
	
  I2C_Send7bitAddress(I2C1,NT3H1101ReadAdd,I2C_Direction_Receiver);//发送从机地址读
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}
	}
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);//收完最后一个数据发NAK
	
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))//等待接收完成
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
      nt3h1101_reg.nt3h1101_status = NT3H1101_ERR;
			return nt3h1101_reg;
		}
	}
	nt3h1101_reg.reg = I2C_ReceiveData(I2C1);
	nt3h1101_reg.nt3h1101_status = NT3H1101_OK;

	I2C_GenerateSTOP(I2C1,ENABLE);//产生停止信号
	
	I2C_AcknowledgeConfig(I2C1,ENABLE);
	
	return nt3h1101_reg;
}

nt3h1101_status_t nt3h1101_write_reg(uint8_t rega,uint8_t mask,uint8_t reg_data)
{
	nt3h1101_timeout = I2C1_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)) //判断总线是否忙
	{
		if((nt3h1101_timeout--) == 0) 
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}
	
	I2C_GenerateSTART(I2C1,ENABLE); //产生起始信号
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}	

	I2C_Send7bitAddress(I2C1,NT3H1101WriteAdd,I2C_Direction_Transmitter); //发送从机地址写
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}	

	I2C_Cmd(I2C1,ENABLE);
	
	I2C_SendData(I2C1,MEMA);//发送MEMA地址
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}
	
	I2C_SendData(I2C1,rega);//发送rega地址
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}

	I2C_SendData(I2C1,mask); //发送mask，mask-把具体需要修改的那几个bit位置1
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}		
	
	I2C_SendData(I2C1,reg_data); //发送需要写入寄存器的数据
	nt3h1101_timeout = I2C1_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((nt3h1101_timeout--) == 0)
		{
			i2c1_recover();
			return NT3H1101_ERR;
		}
	}

	I2C_GenerateSTOP(I2C1,ENABLE); //产生stop信号
	
	return NT3H1101_OK;	
}

void nt3h1101_int_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_NFC_INT,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_PIN_NFCINT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //FD引脚是开漏输出，必须使能上拉电阻
	GPIO_Init(RCC_PORT_NFCINT,&GPIO_InitStruct); 	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PORT_NFC,EXTI_PinSourceNFC);	
	
	EXTI_InitStruct.EXTI_Line = EXTI_LINE_NFC;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI_IRQ_NFC;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = EXTI_NFC_PreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = EXTI_NFC_SubPriority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void nt3h1101_init(void)
{
	nt3h1101_power_ctl_init(); //电源控制引脚初始化
	nt3h1101_int_init(); //中断引脚初始化
}

void nt3h1101_int_enable(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI_IRQ_NFC;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = EXTI_NFC_PreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = EXTI_NFC_SubPriority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);	
}

void nt3h1101_int_disable(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI_IRQ_NFC;
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	
	NVIC_Init(&NVIC_InitStruct);		
}
