/* Includes */
#include "stm32f0xx_gpio.h"
#include "/Users/caseybrittain/STM32F0/lib/project_lib/source/interrupt.c"
#include "/Users/caseybrittain/STM32F0/lib/project_lib/source/delay.c"
#include <string.h>

#define MAX_BUFFER_SIZE 256

typedef struct Buffer
{
	int length;
	uint8_t data[MAX_BUFFER_SIZE];
} Buffer;
static const struct Buffer EmptyBuffer;
Buffer rxBuffer;


/**
**===========================================================================
** Abstract: main program
**===========================================================================
*/

volatile int key_pressed = 0;
 
void configure_button(GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitTypeDef EXTI_InitStructure);
void USART1_Init(uint32_t baudRate);
//////////////////////////////////////////////////////////////////////////////
///
/// \b _write
///
/// @brief writes string to uart
///
/// @param[in] fd file to write -- not used
/// @param[in] s pointer to string to write
/// @param[in] len length of string to write
/// @return number of chars written
///
//////////////////////////////////////////////////////////////////////////////
int write(uint8_t *data, int length);
void clearAllUSARTIRQFlags();
void rxFullPacket(Buffer buffer);
void receivedUSARTData(Buffer buffer);

int main(void)
{
	// Set the delay to system clock.
	TM_Delay_Init(8000000);
	USART1_Init(9600);

	while(1) {
		// write((uint8_t*)"Hello\n", 6);

		TM_DelayMillis(1000);
	}
	return 0;
}

void USART1_IRQHandler(void)
{
	// TODO: Handle buffer overrun.
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		rxBuffer.data[rxBuffer.length] = USART_ReceiveData(USART1);
		rxBuffer.length++;
	}
	// TODO: Better flag handling.
	clearAllUSARTIRQFlags();
	receivedUSARTData(rxBuffer);
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

	/* This was missing from the original code.
	   It turns on the USART interrupts; the NVIC_Init() tells
	   stack what interrupts to setup and how.
	 */
	NVIC_EnableIRQ(USART1_IRQn);
	
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
}

void receivedUSARTData(Buffer buffer) {
	if (buffer.data[buffer.length - 1] == '\r') {
		rxFullPacket(buffer);
	}
}

void rxFullPacket(Buffer buffer) {
	write(buffer.data, buffer.length);
	rxBuffer = EmptyBuffer;
}

int write(uint8_t *data, int length) {
	int i = 0;
	for (; i < length; i++) {
		USART_SendData(USART1, data[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
	}
	return i;
}

void clearAllUSARTIRQFlags(){
	USART_ClearFlag(USART1, USART_IT_WU);
	USART_ClearFlag(USART1, USART_IT_CM);
	USART_ClearFlag(USART1, USART_IT_EOB);
	USART_ClearFlag(USART1, USART_IT_RTO);
	USART_ClearFlag(USART1, USART_IT_CTS);
	USART_ClearFlag(USART1, USART_IT_LBD);
	USART_ClearFlag(USART1, USART_IT_TXE);
	// USART_ClearFlag(USART1, USART_IT_RXNE);
	USART_ClearFlag(USART1, USART_IT_IDLE);
	USART_ClearFlag(USART1, USART_IT_PE);
	USART_ClearFlag(USART1, USART_IT_ERR);
}