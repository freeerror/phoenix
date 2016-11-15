#ifndef __NT3H1101_H__
#define __NT3H1101_H__

#include "sys.h"

#define NT3H1101WriteAdd        0xAA
#define NT3H1101ReadAdd         0xAB

#define NT3H1101OneBlockBytes   16
#define NT3H1101IdBytes         9

typedef enum
{
	NT3H1101_ERR = 0,
	NT3H1101_OK,
}nt3h1101_status_t;

typedef struct
{
	nt3h1101_status_t nt3h1101_status;
	uint8_t block_bytes[NT3H1101OneBlockBytes];
}nt3h1101_result_t;

typedef struct
{
	nt3h1101_status_t nt3h1101_status;
	uint8_t id_bytes[NT3H1101IdBytes];
}nt3h1101_id_t;

typedef struct
{
	nt3h1101_status_t nt3h1101_status;
	uint8_t reg;
}nt3h1101_reg_t;

//extern void nt3h1101_power_ctl_init(void);
extern void nt3h1101_power_on(void);
extern void nt3h1101_power_off(void);
extern nt3h1101_status_t nt3h1101_write_one_block(uint8_t block_add,uint8_t block_bytes[NT3H1101OneBlockBytes]);
extern nt3h1101_result_t nt3h1101_read_one_block(uint8_t block_add);
extern nt3h1101_id_t nt3h1101_get_id(void);
extern nt3h1101_reg_t nt3h1101_read_reg(uint8_t rega);
extern nt3h1101_status_t nt3h1101_write_reg(uint8_t rega,uint8_t mask,uint8_t reg_data);
//extern void nt3h1101_int_init(void);
extern void nt3h1101_init(void);
extern void nt3h1101_int_enable(void);
extern void nt3h1101_int_disable(void);

#endif
