#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "stm32f0xx.h"
#include "stm32f0xx_exti.h"

void gpio_configure_interrupt(uint16_t pin_number, uint8_t edge_selection);
void gpio_enable_interrupt(uint16_t pin_number, uint8_t irq_number);
void gpio_clear_interrupt(uint16_t pin_number);
#endif