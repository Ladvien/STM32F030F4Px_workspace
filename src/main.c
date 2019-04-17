/* Includes */
#include "stm32f0xx_gpio.h"
#include "/home/ladvien/STM32F030F4Px_workspace/lib/project_lib/source/interrupt.c"
#include "/home/ladvien/STM32F030F4Px_workspace/lib/project_lib/source/delay.c"
#include <string.h>


#define MAX_BUFFER_SIZE 256

typedef struct Buffer_st
{
	int size;
	uint8_t data[MAX_BUFFER_SIZE];
} Buffer_st;

Buffer_st receivedDataUART1;
/**
**===========================================================================
** Abstract: main program
**===========================================================================
*/

volatile int key_pressed = 0;
 
void configure_button(GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitTypeDef EXTI_InitStructure);
void USART1_Init(uint32_t baudRate);
void sendDataUART1(Buffer_st buffer);
Buffer_st stringToBuffer(const char* str) ;

int main(void)
{
	// Set the delay to system clock.
	TM_Delay_Init(8000000);

	while(1) {
		const char* test = "Hello\n";
		Buffer_st buffer = stringToBuffer(test);
		// Buffer_st buffer = {6, "Hello\n"};
		USART1_Init(9600);
		sendDataUART1(buffer);
		TM_DelayMillis(1000);
	}
	return 0;
}

void resetReceivedDataBufferUART1(void)
{
	receivedDataUART1.size = 0;
	memset(receivedDataUART1.data, MAX_BUFFER_SIZE, 0);
}

void sendDataUART1(Buffer_st buffer)
{
	uint8_t cpt;
	for (cpt = 0; cpt < buffer.size; cpt++)
	{
		USART_SendData(USART1, buffer.data[cpt]);
		//Loop until the end of transmission
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);
	}
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		receivedDataUART1.data[receivedDataUART1.size++] = USART_ReceiveData(USART1);
	}
}

/*****************************************************
 * Initialize USART1: enable interrupt on reception
 * of a character
 ****************************************************
 USART code from:
 https://community.st.com/s/question/0D50X00009XkghE/stm32f0-uart-not-sending-data
 */
void USART1_Init(uint32_t baudRate)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//UART init
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	
	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	
	/* Configure USART Tx, Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART configuration */
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	resetReceivedDataBufferUART1();
}

Buffer_st stringToBuffer(const char* str) {
	Buffer_st buffer = {6, *str};
	return buffer;
}