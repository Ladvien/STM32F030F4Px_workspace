#include "stm32f0xx.h"
#include "stm32f0xx_exti.h"
#include "interrupt.h"
void gpio_configure_interrupt(uint16_t pin_number, uint8_t edge_selection) {
    if (edge_selection == EXTI_Trigger_Rising) {
        EXTI->RTSR |= 1 << pin_number;
    }
    else if (edge_selection == EXTI_Trigger_Falling){
        EXTI->FTSR |= 1 << pin_number;
    }
    else if (edge_selection == EXTI_Trigger_Rising_Falling) {
        EXTI->RTSR |= 1 << pin_number;
        EXTI->FTSR |= 1 << pin_number;
    }
}

void gpio_enable_interrupt(uint16_t pin_number, uint8_t irq_number) {
    EXTI->IMR |= 1 << pin_number;
    NVIC_EnableIRQ(irq_number);
}

void gpio_clear_interrupt(uint16_t pin_number) {
    if(EXTI->PR & (1 << pin_number)) {
        EXTI->PR = pin_number;
    }
}

