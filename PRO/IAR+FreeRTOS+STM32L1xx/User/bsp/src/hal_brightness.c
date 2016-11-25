#include "includes.h"

opt3001_status_t hal_opt3001_reg_init(void)
{
  opt3001_status_t result;
  result =  opt3001_reg_init();
  return result;
}

void hal_opt3001_int_init(void)
{
  opt3001_int_init();
}

opt3001_status_t hal_opt3001_alarm_init(float opt3001_low_limit,float opt3001_high_limit)
{
  opt3001_status_t result;
  result = opt3001_alarm_init(opt3001_low_limit,opt3001_high_limit);
  return result;
}

opt3001_result_t hal_opt3001_get_lux()
{
  opt3001_result_t result;
  result = opt3001_get_lux();
  return result;
}

opt3001_result_t hal_opt3001_int_get_lux(void)
{
  opt3001_result_t result;
  result = opt3001_int_get_lux();
  return result;
}









