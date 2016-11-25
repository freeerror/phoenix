#include "includes.h"

void hal_nfc_init(void)
{
  nt3h1101_init();
}

void hal_nfc_int_enable(void)
{
  nt3h1101_int_enable();
}

void hal_nfc_int_disable(void)
{
  nt3h1101_int_disable();
}

void hal_nfc_power_on(void)
{
  nt3h1101_power_on();
}

void hal_nfc_power_off(void)
{
  nt3h1101_power_off();
}

//nfc写一个block（16个字节）函数，一次只能操作一个block
nt3h1101_status_t hal_nfc_write_one_block(uint8_t block_add,uint8_t block_bytes[NT3H1101OneBlockBytes])
{
  nt3h1101_status_t nt3h1101_status;
  
  nt3h1101_status = nt3h1101_write_one_block(block_add,block_bytes);
  
  return nt3h1101_status;
}

//nfc读一个block（16个字节）函数，一次只能操作一个block
nt3h1101_result_t hal_nfc_read_one_block(uint8_t block_add)
{
  nt3h1101_result_t nt3h1101_result;
  
  nt3h1101_result = nt3h1101_read_one_block(block_add);
  
  return nt3h1101_result;
}

//获取nfc ID
nt3h1101_id_t hal_nfc_get_id(void)
{
  nt3h1101_id_t nt3h1101_id;
  
  nt3h1101_id = nt3h1101_get_id();
  
  return nt3h1101_id;
}