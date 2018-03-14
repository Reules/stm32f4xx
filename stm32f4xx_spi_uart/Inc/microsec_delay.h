#ifndef MICRO_DELAY_H
#define MICRO_DELAY_H
#include "stm32f4xx_hal.h"

uint32_t getUs(void);

void delayUs(uint16_t micros);

#endif
