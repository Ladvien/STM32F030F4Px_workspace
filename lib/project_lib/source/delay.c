#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "delay.h"

uint32_t multiplier;

void TM_Delay_Init(uint32_t clock_freq) {
    RCC_ClocksTypeDef RCC_Clocks;
    
    /* Get system clocks */
    RCC_GetClocksFreq(&RCC_Clocks);
    
    /* For 1 us delay, we need to divide with by xMHZ */
    multiplier = RCC_Clocks.HCLK_Frequency / clock_freq;
}

void TM_DelayMicros(uint32_t micros) {
    /* Multiply micros with multipler */
    /* Substract 10 */
    micros = micros * multiplier - 10;
    /* 4 cycles for one loop */
    while (micros--);
}
 
void TM_DelayMillis(uint32_t millis) {
    /* Multiply millis with multipler */
    /* Substract 10 */
    millis = 1000 * millis * multiplier - 10;
    /* 4 cycles for one loop */
    while (millis--);
}