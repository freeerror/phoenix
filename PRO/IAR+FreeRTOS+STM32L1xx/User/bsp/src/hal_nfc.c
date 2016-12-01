#include "includes.h"

#define NFC_RETRY_TIMES  2 //如果读取失败，就重新读取3次，连同第一次，总共读取了4次

/*
*********************************************************************************************************
*	函 数 名: void hal_nfc_init(void)
*	功能说明: 初始化电源控制引脚及中断检测引脚，然后开启nfc模块电源
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void hal_nfc_init(void)
{
  nt3h1101_init();
  hal_nfc_power_on();//nfc初始化以后就开启nfc电源，否则没法检测到nfc中断
}

/*
*********************************************************************************************************
*	函 数 名: void hal_nfc_int_enable(void)
*	功能说明: 使能MCU中断检测引脚
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void hal_nfc_int_enable(void)
{
  nt3h1101_int_enable();
}

/*
*********************************************************************************************************
*	函 数 名: void hal_nfc_int_disable(void)
*	功能说明: 失能MCU中断检测引脚
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void hal_nfc_int_disable(void)
{
  nt3h1101_int_disable();
}

/*
*********************************************************************************************************
*	函 数 名: void hal_nfc_power_on(void)
*	功能说明: 开启nfc模块电源，同时配置nfc芯片的中断输出引脚
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void hal_nfc_power_on(void)
{
  nt3h1101_power_on();
}

/*
*********************************************************************************************************
*	函 数 名: void hal_nfc_power_off(void)
*	功能说明: 关闭nfc模块的电源
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void hal_nfc_power_off(void)
{
  nt3h1101_power_off();
}

/*
*********************************************************************************************************
*	函 数 名: nt3h1101_status_t hal_nfc_write_one_block(uint8_t block_add,uint8_t block_bytes[NT3H1101OneBlockBytes])
*	功能说明: 向nfc芯片内写一个block的内容，如果第一次写失败，则再次写三次
*	形    参: uint8_t block_add:输入参数，需要写入的block的地址
*             uint8_t block_bytes[NT3H1101OneBlockBytes]:输入参数，写到的block内的内容
*	返 回 值: 写入是否成功
*********************************************************************************************************
*/
nt3h1101_status_t hal_nfc_write_one_block(uint8_t block_add,uint8_t block_bytes[NT3H1101OneBlockBytes])
{
  nt3h1101_status_t nt3h1101_status;
  uint8_t retry = 0;
  
  nt3h1101_status = nt3h1101_write_one_block(block_add,block_bytes);//先写一次

  while(nt3h1101_status == NT3H1101_ERR)//如果写失败
  {
    hal_nfc_power_off();//断电
    vTaskDelay(50 / portTICK_RATE_MS);//延时50ms
    hal_nfc_power_on();//上电
    nt3h1101_status = nt3h1101_write_one_block(block_add,block_bytes); //再次写
    if(++retry > NFC_RETRY_TIMES)//再次写三次还失败
    {
        return NT3H1101_ERR;
    }
  }

  vTaskDelay(10 / portTICK_RATE_MS);//i2c读写操作需要一段时间
  
  return nt3h1101_status;
}

/*
*********************************************************************************************************
*	函 数 名: nt3h1101_result_t hal_nfc_read_one_block(uint8_t block_add)
*	功能说明: 读nfc芯片内一个block的内容,如果第一次读取失败则再次重新读取3次
*	形    参: uint8_t block_add:输入参数，需要读取的block的地址
*	返 回 值: 读取是否成功及读取到的block内的内容
*********************************************************************************************************
*/
nt3h1101_result_t hal_nfc_read_one_block(uint8_t block_add)
{
    nt3h1101_result_t nt3h1101_result;
    uint8_t retry = 0;

    nt3h1101_result = nt3h1101_read_one_block(block_add);//先读取一次

    while(nt3h1101_result.nt3h1101_status == NT3H1101_ERR)//如果读取失败
    {
        hal_nfc_power_off(); //断电
        vTaskDelay(50 / portTICK_RATE_MS); //延时50ms
        hal_nfc_power_on(); //上电
        nt3h1101_result = hal_nfc_read_one_block(block_add); //再次读取
        if(++retry > NFC_RETRY_TIMES)//再次读取三次还失败 
        {
            nt3h1101_result.nt3h1101_status = NT3H1101_ERR;
            return nt3h1101_result;
        }
    }

    vTaskDelay(10 / portTICK_RATE_MS);//i2c读写操作需要一段时间

    return nt3h1101_result;
}

/*
*********************************************************************************************************
*	函 数 名: nt3h1101_id_t hal_nfc_get_id(void)
*	功能说明: 读nfc ID
*	形    参: 无
*	返 回 值: nfc芯片id及获取id是否成功
*********************************************************************************************************
*/
nt3h1101_id_t hal_nfc_get_id(void)
{
  nt3h1101_id_t nt3h1101_id;

  //hal_nfc_power_on();
  
  nt3h1101_id = nt3h1101_get_id();

  //hal_nfc_power_off();
  
  return nt3h1101_id;
}