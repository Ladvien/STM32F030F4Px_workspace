/* Includes */
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

/* Private typedef */
/* Private define */
/* Private macro */
#define LED_PORT GPIOA
#define LED1 GPIO_Pin_4
#define LED2 GPIO_Pin_2

#define KEY_PORT GPIOC
#define KEY GPIO_Pin_5

/* Private variables */
/* Private function prototypes */
/* Private functions */
/* Global variables */
uint32_t timer = 0;
uint8_t timerFlag =  0;
/**

/**
**===========================================================================
** Abstract: main program
**===========================================================================
*/

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	SysTick_Config(4800); /* 0.1 ms = 100us if clock frequency 12 MHz */

	SystemCoreClockUpdate();

	/* GPIOA-C Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* Configure Button input
	* */
	GPIO_InitStructure.GPIO_Pin = KEY;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);

	/* 
	* Setup LED (PA0)
	*/
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
	int index = 0;
	while (1)
	{
		/* Toggle LED1 */
		GPIO_SetBits(LED_PORT, LED1);
		for(int i = 0; i < 500000; i++){
			;
		}

		printf('A');
		index++;
		GPIO_ResetBits(LED_PORT, LED1);
		for(int i = 0; i < 500000; i++){
			;;
		}

	}

return 0;
}
