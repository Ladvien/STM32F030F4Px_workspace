/**********************************************************************************************************************
/**
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


/* Private typedef */
/* Private define */
/* STM32F0_DISCOVERY board definitions */
#define LED_PORT GPIOA
#define LED1 GPIO_Pin_4
#define LED2 GPIO_Pin_2

#define KEY_PORT GPIOC
#define KEY GPIO_Pin_5

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */
/* Global variables */
uint32_t timer=0;
uint8_t timerFlag=0;
/**

/**
**===========================================================================
** Abstract: main program
**===========================================================================
*/
int main(void)
{
uint32_t ii = 0;
GPIO_InitTypeDef GPIO_InitStructure;

/* TODO - Add your application code here */
SysTick_Config(4800); /* 0.1 ms = 100us if clock frequency 12 MHz */

SystemCoreClockUpdate();
ii = SystemCoreClock; /* This is a way to read the System core clock */
ii = 0;

/* GPIOA-C Periph clock enable */
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

/* Configure
* PA0 USER Button input
* */
GPIO_InitStructure.GPIO_Pin = KEY;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(KEY_PORT, &GPIO_InitStructure);

/* Configure PC8 and PC9 in output pushpull mode
* PC8 = LD3 Green LED
* PC9 = LD4 Blue LED
* */
GPIO_InitStructure.GPIO_Pin = LED1 | LED2;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(LED_PORT, &GPIO_InitStructure);

GPIO_SetBits(LED_PORT, LED1);
GPIO_ResetBits(LED_PORT, LED1);
LED_PORT->BSRR = LED2;
LED_PORT->BRR = LED2;
	int i=0;
	while (1)
	{
		/* Toggle LED1 */
		GPIO_SetBits(LED_PORT, LED1);
		for(int i = 0; i < 500000; i++){
			;
		}

		GPIO_ResetBits(LED_PORT, LED1);
		for(int i = 0; i < 500000; i++){
			;
		}

	}
return 0;
}
