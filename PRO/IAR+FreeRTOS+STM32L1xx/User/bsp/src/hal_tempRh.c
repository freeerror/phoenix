#include "includes.h"

sht20_result_t hal_get_tempRH(void)
{
  sht20_result_t sht20_result;
  
  sht20_result = getTempRh();
  
  return sht20_result;
}