/* Includes */
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "/home/ladvien/STM32F030F4Px_workspace/lib/project_lib/source/interrupt.c"
#include "/home/ladvien/STM32F030F4Px_workspace/lib/project_lib/source/delay.c"

/* 
	Macros
*/

#define LED_PORT GPIOA
#define LED1 GPIO_Pin_4
#define LED2 GPIO_Pin_0

#define KEY_PORT GPIOA
#define KEY GPIO_Pin_1

/**
**===========================================================================
** Abstract: main program
**===========================================================================
*/

volatile int key_pressed = 0;

void configure_button(GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitTypeDef EXTI_InitStructure);


int main(void)
{
	// Set the delay to system clock.
	TM_Delay_Init(8000000);

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_StructInit(&EXTI_InitStructure);

	// GPIOA Periph clock enable 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	configure_button(GPIO_InitStructure, EXTI_InitStructure);
	/*
	* Setup LED (PA0)
	* GPIO speed cannot exceed 1/2 the clock.
	*/
	GPIO_InitStructure.GPIO_Pin = LED2 | LED1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	GPIO_SetBits(LED_PORT, LED1);
	while (1)
	{
		if(key_pressed) {
			key_pressed = DISABLE;
			// Toggle LED1 *
			GPIO_ResetBits(LED_PORT, LED1);
			TM_DelayMillis(1000);
			GPIO_SetBits(LED_PORT, LED1);
		}
	}
	return 0;
}

void EXTI0_1_IRQHandler(void) {
	// Clear interrupt on PIN_0.
	if(EXTI_GetITStatus(KEY)) {
		EXTI_ClearITPendingBit(KEY);
		// Set key_pressed.
		key_pressed = ENABLE;
	}
}

void configure_button(GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitTypeDef EXTI_InitStructure) {
		/* Configure Button input
	 * GPIO speed cannot exceed 1/2 the clock.
	*/
	GPIO_InitStructure.GPIO_Pin = KEY;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);

	// /*
	// 	Configure button interrupt/
	// */
	SYSCFG_EXTILineConfig((uint8_t)KEY_PORT, KEY);
	EXTI_InitStructure.EXTI_Line = KEY;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);

	// /* Add IRQ vector to NVIC */
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}