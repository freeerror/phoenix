#ifndef __HAL_ACC_SENSOR__
#define __HAL_ACC_SENSOR__

void hal_acc_init(void);

void hal_acc_int_init(void);//配置中断

adxl345_result_t hal_getAcceleration(void);//读取三轴加速度值

adxl345_status_t hal_observeActivity(int8_t thresh , bool x, bool y , bool z);//配置activity中断

adxl345_status_t hal_observeFreefall(void);//配置freefall中断

adxl345_int_result_t hal_act_freefall_int_process(void);//查询中断标志，处理中断

#endif