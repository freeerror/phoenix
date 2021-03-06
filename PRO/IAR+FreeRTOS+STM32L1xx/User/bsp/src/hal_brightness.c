#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名:opt3001_status_t hal_opt3001_reg_init(void)
*	功能说明:初始化opt3001的寄存器，自动量程，转换时间100ms，连续转化模式，中断输出锁存，直到用户读取
*            configuration寄存器，当有中断时中断引脚输出低电平
*	形    参:无  
*	返 回 值:配置是否成功 
*********************************************************************************************************
*/
opt3001_status_t hal_opt3001_reg_init(void)
{
  opt3001_status_t result;
  result =  opt3001_reg_init();
  return result;
}

/*
*********************************************************************************************************
*	函 数 名:void hal_opt3001_int_init(void)
*	功能说明:初始化与opt3001中断引脚相连的MCU的引脚
*	形    参:无  
*	返 回 值:无 
*********************************************************************************************************
*/
void hal_opt3001_int_init(void)
{
  opt3001_int_init();
}

/*
*********************************************************************************************************
*	函 数 名:opt3001_status_t hal_opt3001_alarm_init(float opt3001_low_limit,float opt3001_high_limit)
*	功能说明:初始opt3001的上下限报警阈值
*	形    参:float opt3001_low_limit:下限阈值；float opt3001_high_limit:上限阈值
*	返 回 值:操作是否成功 
*********************************************************************************************************
*/
opt3001_status_t hal_opt3001_alarm_init(float opt3001_low_limit,float opt3001_high_limit)
{
  opt3001_status_t result;
  result = opt3001_alarm_init(opt3001_low_limit,opt3001_high_limit);
  return result;
}

/*
*********************************************************************************************************
*	函 数 名:opt3001_result_t hal_opt3001_get_lux(void)
*	功能说明:获取当前光强值
*	形    参:无
*	返 回 值:光强值，单位lux及操作是否成功
*********************************************************************************************************
*/
opt3001_result_t hal_opt3001_get_lux(void)
{
  opt3001_result_t result;
  result = opt3001_get_lux();
  return result;
}

/*
*********************************************************************************************************
*	函 数 名:opt3001_result_t hal_opt3001_int_get_lux(void)
*	功能说明:中断发生后获取光强值，同时读取configuration寄存器，以便下次中断触发
*	形    参:无
*	返 回 值:光强值，单位lux及操作是否成功
*********************************************************************************************************
*/
opt3001_result_t hal_opt3001_int_get_lux(void)
{
  opt3001_result_t result;
  result = opt3001_int_get_lux();
  return result;
}









