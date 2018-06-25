#ifndef FSK_CRC16
#define FSK_CRC16
#include "stm32f4xx_hal.h"

#define CRC16 0x8005

void fskCRC16(uint16_t data[], uint8_t size);

#endif
