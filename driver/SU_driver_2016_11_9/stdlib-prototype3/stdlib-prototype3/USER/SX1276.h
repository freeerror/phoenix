#ifndef __SX1276_H__
#define __SX1276_H__

#include "sys.h"

#define sx1276_cs PBout(6)

extern uint8_t sx1276_read_version(uint8_t reg_add);

#endif
