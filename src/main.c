/**********************************************************************************************************************
**
** File : main.c
** Abstract : main function.
** Functions : main
** Environment : Atollic TrueSTUDIO(R)
** Distribution: The file is distributed “as is,” without any warranty
** of any kind.
** (c)Copyright Atollic AB.
** You may use this file as-is or modify it according to the needs of your project. This file may only be
** built (assembled or compiled and linked) using the Atollic TrueSTUDIO(R) product. The use of this file
** together with other tools than Atollic TrueSTUDIO(R) is not permitted.
**********************************************************************************************************************
*/

/* Includes */
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "/Users/caseybrittain/STM32F0/lib/project_lib/source/interrupt.c"
#include "/Users/caseybrittain/STM32F0/lib/project_lib/source/delay.c"

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

int main(void)
{
	TM_Delay_Init(8000000);

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	EXTI_StructInit(&EXTI_InitStructure);

	// /* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

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
	/* PA6 is connected to EXTI0_1_IRQn that uses EXTI0_1_IRQHandler
	 * GPIO speed cannot exceed 1/2 the clock.
	 * */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);

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
	GPIO_ResetBits(LED_PORT, LED1);
	while (1)
	{
		if(key_pressed) {
			/* Toggle LED1 */
			GPIO_SetBits(LED_PORT, LED1);
			TM_DelayMillis(1000);
			GPIO_ResetBits(LED_PORT, LED1);
			TM_DelayMillis(1000);
			key_pressed = DISABLE;
		}
	}
	return 0;
}

void EXTI0_1_IRQHandler(void) {
	key_pressed = ENABLE;
//	EXTI->PR |= EXTI_PR_PR0;
	gpio_clear_interrupt(KEY);
//	EXTI_ClearFlag();
	EXTI_ClearITPendingBit(KEY);
//	NVIC_ClearPendingIRQ(EXTI0_1_IRQn);
}