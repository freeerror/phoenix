#include "includes.h"

/* 定义每个串口结构体变量 */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* 接收缓冲区 */
	//static uint8_t g_TxBuf2Temp[128];
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART1_FIFO_EN == 1
	#define RCC_USART1        RCC_APB2Periph_USART1
	#define RCC_USART1_PORT   RCC_AHBPeriph_GPIOA
	#define GPIO_PORT_USART1  GPIOA
	#define GPIO_PIN_TX1      GPIO_Pin_9
	#define GPIO_PIN_RX1      GPIO_Pin_10
	#define GPIO_PinSourceTX1 GPIO_PinSource9
	#define GPIO_PinSourceRX1 GPIO_PinSource10
#endif

#if UART2_FIFO_EN == 1
	#define RCC_USART2        RCC_APB1Periph_USART2
	#define RCC_USART2_PORT   RCC_AHBPeriph_GPIOA
	#define GPIO_PORT_USART2  GPIOA
	#define GPIO_PIN_TX2      GPIO_Pin_2
	#define GPIO_PIN_RX2      GPIO_Pin_3
	#define GPIO_PinSourceTX2 GPIO_PinSource2
	#define GPIO_PinSourceRX2 GPIO_PinSource3
#endif

#if UART3_FIFO_EN == 1

  #if 0
	#define RCC_USART3        RCC_APB1Periph_USART3
	#define RCC_USART3_PORT   RCC_AHBPeriph_GPIOD
	#define GPIO_PORT_USART3  GPIOD
	#define GPIO_PIN_TX3      GPIO_Pin_8
	#define GPIO_PIN_RX3      GPIO_Pin_9
	#define GPIO_PinSourceTX3 GPIO_PinSource8
	#define GPIO_PinSourceRX3 GPIO_PinSource9
  #else
  #define RCC_USART3        RCC_APB1Periph_USART3
	#define RCC_USART3_PORT   RCC_AHBPeriph_GPIOB
	#define GPIO_PORT_USART3  GPIOB
	#define GPIO_PIN_TX3      GPIO_Pin_10
	#define GPIO_PIN_RX3      GPIO_Pin_11
	#define GPIO_PinSourceTX3 GPIO_PinSource10
	#define GPIO_PinSourceRX3 GPIO_PinSource11
  #endif
  
#endif

#if UART1_FIFO_EN == 1

  #define UART1_TX_DMA_Channel  DMA1_Channel4
  #define UART1_TX_DMA_Flag     DMA1_FLAG_TC4
  #define UART1_DR_ADDRESS      0x40013804
  
#endif

#if UART2_FIFO_EN == 1

  #define UART2_TX_DMA_Channel  DMA1_Channel7
  #define UART2_TX_DMA_Flag     DMA1_FLAG_TC7
  #define UART2_DR_ADDRESS      0x40004404
	
#endif

#if UART3_FIFO_EN == 1

  #define UART3_TX_DMA_Channel  DMA1_Channel2
  #define UART3_TX_DMA_Flag  DMA1_FLAG_TC2
  #define UART3_DR_ADDRESS      0x40004804
	
#endif


static void UartVarInit(void);
static void InitHardUart(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);
static void ConfigUartNVIC(void);
static UART_T *ComToUart(COM_PORT_E _ucPort);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* 必须先初始化全局变量,再配置硬件 */

	InitHardUart();		/* 配置串口的硬件参数(波特率等) */

	ConfigUartNVIC();	/* 配置串口中断 */
}

/*
*********************************************************************************************************
*	函 数 名: UARTx_TX_DMA_Init(uint32_t UARTxDRAdd,uint32_t TxDMAMemBaseAdd,DMA_Channel_TypeDef * DMAx_Channely)
*	功能说明: 初始化串口X的TX的DMA通道
*	形    参: UARTxDRAdd:串口x的Tx的发送寄存器地址 
            TxDMAMemBaseAdd:TX的DMA传输的内存基地址
            DMAx_Channely：串口x所对应的DMA通道
*	返 回 值: 无
*********************************************************************************************************
*/
void UARTx_TX_DMA_Init(uint32_t UARTxDRAdd,uint32_t TxDMAMemBaseAdd,DMA_Channel_TypeDef * DMAx_Channely)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMAx_Channely);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = UARTxDRAdd;
	DMA_InitStructure.DMA_MemoryBaseAddr = TxDMAMemBaseAdd;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 64;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMAx_Channely,&DMA_InitStructure);	
}


/*
*********************************************************************************************************
*	函 数 名: UARTx_TX_DMA_Enable(UART_T *pUart,uint16_t _len)
*	功能说明: 初始化串口X的TX的DMA通道
*	形    参: 
*	返 回 值: 无
*********************************************************************************************************
*/

void UARTx_TX_DMA_Enable(UART_T *pUart,uint16_t _len)
{
  if(pUart->uart == USART1)
  {
  	DMA_Cmd(UART1_TX_DMA_Channel,DISABLE);
	DMA_SetCurrDataCounter(UART1_TX_DMA_Channel,_len);
	DMA_Cmd(UART1_TX_DMA_Channel,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	while(DMA_GetFlagStatus(UART1_TX_DMA_Flag) == RESET);
	DMA_ClearFlag(UART1_TX_DMA_Flag);//千万不能忘了清标志位
	comClearTxFifo(COM1);
  }
  else if(pUart->uart == USART2)
  {		
    DMA_Cmd(UART2_TX_DMA_Channel,DISABLE);
	DMA_SetCurrDataCounter(UART2_TX_DMA_Channel,_len);
	DMA_Cmd(UART2_TX_DMA_Channel,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	while(DMA_GetFlagStatus(UART2_TX_DMA_Flag) == RESET);
	DMA_ClearFlag(UART2_TX_DMA_Flag);//千万不能忘了清标志位
	comClearTxFifo(COM2);
  }
  else if(pUart->uart == USART3)
  {		
    DMA_Cmd(UART3_TX_DMA_Channel,DISABLE);
	DMA_SetCurrDataCounter(UART3_TX_DMA_Channel,_len);
    DMA_Cmd(UART3_TX_DMA_Channel,ENABLE);
    USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
    while(DMA_GetFlagStatus(UART3_TX_DMA_Flag) == RESET);
    DMA_ClearFlag(UART3_TX_DMA_Flag);//千万不能忘了清标志位
    comClearTxFifo(COM3);
  }
}

/*
*********************************************************************************************************
*	函 数 名: Uart1_TX_DMA_Enable(uint16_t _len)
*	功能说明: 开启一次串1TX的DMA传输
*	形    参:  _len:一次所要传输的数据的长度
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart1_TX_DMA_Enable(uint16_t _len)
{
	DMA_Cmd(DMA1_Channel4,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4,_len);
	DMA_Cmd(DMA1_Channel4,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	comClearTxFifo(COM1);
}

/*
*********************************************************************************************************
*	函 数 名: Uart3_TX_DMA_Enable(uint16_t _len)
*	功能说明: 开启一次串3TX的DMA传输
*	形    参:  _len:一次所要传输的数据的长度
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart3_TX_DMA_Enable(uint16_t _len)
{
	DMA_Cmd(DMA1_Channel2,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel2,_len);
	DMA_Cmd(DMA1_Channel2,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	comClearTxFifo(COM3);
}

/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为UART指针
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: uart指针
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: comSendBuf
*	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由DMA传输到串口
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _ucaBuf: 待发送的数据缓冲区
*			  _usLen : 数据长度
*	返 回 值: 无
* 注：当发送数据比较长时，两次调用之间必须要有一定的时间间隔，以等待DMA传输完成
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	if (pUart->SendBefor != 0)
	{
		pUart->SendBefor();	
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	函 数 名: comSendChar
*	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _ucByte: 待发送的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	/* 写一个字节到USARTx */
	USART_SendData(pUart->uart,_ucByte);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(pUart->uart, USART_FLAG_TC) == RESET);

}

/*
*********************************************************************************************************
*	函 数 名: comGetChar
*	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return 0;
	}

	return UartGetChar(pUart, _pByte);
}

/*
*********************************************************************************************************
*	函 数 名: comClearTxFifo
*	功能说明: 清零串口发送缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usTxWrite = 0;
	pUart->usTxRead = 0;
	pUart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: comClearRxFifo
*	功能说明: 清零串口接收缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetUart1Baud
*	功能说明: 修改UART1波特率
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetUart1Baud(uint32_t _baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = _baud;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetUart2Baud
*	功能说明: 修改UART2波特率
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetUart2Baud(uint32_t _baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = _baud;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
}


/*
*********************************************************************************************************
*	函 数 名: UartVarInit
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						/* STM32 串口设备 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* 发送缓冲区指针 */
	g_tUart1.pRxBuf = g_RxBuf1;					/* 接收缓冲区指针 */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart1.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart1.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart1.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart1.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart1.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart1.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart1.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart1.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart1.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						/* STM32 串口设备 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* 发送缓冲区指针 */
	g_tUart2.pRxBuf = g_RxBuf2;					/* 接收缓冲区指针 */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart2.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart2.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart2.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart2.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart2.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart2.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart2.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart2.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart2.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						/* STM32 串口设备 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* 发送缓冲区指针 */
	g_tUart3.pRxBuf = g_RxBuf3;					/* 接收缓冲区指针 */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart3.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart3.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart3.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart3.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart3.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart3.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart3.SendBefor = 0;		/* 发送数据前的回调函数 */
	g_tUart3.SendOver = 0;			/* 发送完毕后的回调函数 */
	g_tUart3.ReciveNew = 0;		/* 接收到新数据后的回调函数 */
#endif
}

/*
*********************************************************************************************************
*	函 数 名: InitHardUart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

#if UART1_FIFO_EN == 1		/* 串口1 TX = PA9   RX = PA10 或 TX = PB6   RX = PB7*/

	RCC_AHBPeriphClockCmd(RCC_USART1_PORT, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_PORT_USART1, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART1,GPIO_PinSourceTX1,GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_USART1, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART1,GPIO_PinSourceRX1,GPIO_AF_USART1);
	
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */

	USART_Cmd(USART1, ENABLE);		/* 使能串口 */

	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	
	UARTx_TX_DMA_Init(UART1_DR_ADDRESS,(uint32_t)(g_TxBuf1),UART1_TX_DMA_Channel);
#endif

#if UART2_FIFO_EN == 1		/* 串口2 TX = PA2， RX = PA3  */

	RCC_AHBPeriphClockCmd(RCC_USART2_PORT, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_PORT_USART2, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART2,GPIO_PinSourceTX2,GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_USART2, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART2,GPIO_PinSourceRX2,GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = UART2_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	
	USART_Cmd(USART2, ENABLE);		/* 使能串口 */

	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	
	UARTx_TX_DMA_Init(UART2_DR_ADDRESS,(uint32_t)(g_TxBuf2),UART2_TX_DMA_Channel);
#endif

#if UART3_FIFO_EN == 1			/* 串口3 TX = PB10   RX = PB11 */

	RCC_AHBPeriphClockCmd(RCC_USART3_PORT, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_PORT_USART3, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART3,GPIO_PinSourceTX3,GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_USART3, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART3,GPIO_PinSourceRX3,GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate = UART3_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	
	USART_Cmd(USART3, ENABLE);		/* 使能串口 */

	USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	
	UARTx_TX_DMA_Init(UART3_DR_ADDRESS,(uint32_t)(g_TxBuf3),UART3_TX_DMA_Channel);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: ConfigUartNVIC
*	功能说明: 配置串口硬件中断.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#if UART1_FIFO_EN == 1
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART2_FIFO_EN == 1
	/* 使能串口2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART3_FIFO_EN == 1
	/* 使能串口3中断t */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: UartSend
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* 如果发送缓冲区已经满了，则等待缓冲区空 */
	#if 0

		while (1)
		{
			uint16_t usRead;

			DISABLE_INT();
			usRead = _pUart->usTxRead;
			ENABLE_INT();

			if (++usRead >= _pUart->usTxBufSize)
			{
				usRead = 0;
			}

			if (usRead != _pUart->usTxWrite)
			{
				break;
			}
		}
	#else
		/* 当 _pUart->usTxBufSize == 1 时, 下面的函数会死掉(待完善) */
		while (1)
		{
			__IO uint16_t usCount;

			usCount = _pUart->usTxCount;

			if (usCount < _pUart->usTxBufSize)
			{
				break;
			}
		}
	#endif

		/* 将新数据填入发送缓冲区 */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;
	}
	
    UARTx_TX_DMA_Enable(_pUart,_usLen);
}

/*
*********************************************************************************************************
*	函 数 名: UartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	ENABLE_INT();

	/* 如果读和写索引相同，则返回0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* 已经没有数据 */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* 从串口接收FIFO取1个数据 */

		/* 改写FIFO读索引 */
		DISABLE_INT();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		ENABLE_INT();
		return 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{
	/* 处理接收中断  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		/* 从串口接收数据寄存器读取数据存放到接收FIFO */
		uint8_t ch;

		ch = USART_ReceiveData(_pUart->uart);

		_pUart->pRxBuf[_pUart->usRxWrite] = ch;
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
		}

    if (_pUart->ReciveNew)
		{
			_pUart->ReciveNew(ch);
		}
	}

}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}
#endif

#if UART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
	UartIRQ(&g_tUart3);
}
#endif

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 0	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
	comSendChar(COM2, ch);

	return ch;
#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
	/* 写一个字节到USART2 */
	USART_SendData(USART2, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 1	/* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
	uint8_t ucData;

	while(comGetChar(COM2, &ucData) == 0);

	return ucData;
#else
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART2);
#endif
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
