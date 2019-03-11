#ifndef _DELAY_H_
#define _DELAY_H_

#include "delay.h"
#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"

// Developed from the following article:
// https://stm32f4-discovery.net/2014/09/precise-delay-counter/

uint32_t _multiplier;
void TM_Delay_Init(uint32_t clock_freq);
void TM_DelayMicros(uint32_t micros);
void TM_DelayMillis(uint32_t millis);

#endif