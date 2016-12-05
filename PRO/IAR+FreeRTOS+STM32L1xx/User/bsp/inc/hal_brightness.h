#ifndef __HAL_BRIGHTNESS_H__
#define __HAL_BRIGHTNESS_H__

extern opt3001_status_t hal_opt3001_reg_init(void);

void hal_opt3001_int_init(void);

opt3001_status_t hal_opt3001_alarm_init(float opt3001_low_limit,float opt3001_high_limit);

opt3001_result_t hal_opt3001_get_lux(void);

opt3001_result_t hal_opt3001_int_get_lux(void);

#endif
