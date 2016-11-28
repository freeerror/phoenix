#include "includes.h"

void hal_acc_int_init(void)//中断初始化
{
  adxl345_low_level_init();
}

adxl345_result_t hal_getAcceleration(void)//读取三轴加速度值
{
  adxl345_result_t adxl345_result;
  
  adxl345_result = getAcceleration();
  
  return adxl345_result;
}

adxl345_status_t hal_observeActivity(int8_t thresh , bool x, bool y , bool z)
{
  adxl345_status_t adxl345_status;
  
  adxl345_status = observeActivity(thresh,x,y,z);
  
  return adxl345_status;
}

adxl345_status_t hal_observeFreefall(void)
{
  adxl345_status_t adxl345_status;
  
  adxl345_status = observeFreefall();
  
  return adxl345_status;  
}

adxl345_int_result_t hal_act_freefall_int_process(void)
{
  adxl345_int_result_t adxl345_int_result;
  
  adxl345_int_result = adxl345_act_freefall_int_process();
  
  return adxl345_int_result;  
}

void hal_acc_init(void)
{
    adxl345_init();
}