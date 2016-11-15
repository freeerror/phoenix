#ifndef __I2C_H__
#define __I2C_H__

#include "sys.h"

#define I2C1_FLAG_TIMEOUT ((uint32_t)0x1000)
#define I2C1_LONG_TIMEOUT ((uint32_t)(10 * I2C1_FLAG_TIMEOUT))

extern void i2c1_init(void);
extern void i2c1_recover(void);

#endif
