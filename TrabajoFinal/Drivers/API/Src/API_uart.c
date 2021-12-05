/*
 * API_uart.c
 *
 *  Created on: Nov 22, 2021
 *      Author: lautaro
 */

#include "stm32f4xx_hal.h"          /* <- HAL include */
#include "stm32f4xx_nucleo_144.h"   /* <- BSP include */
#include <string.h>
#include "API_uart.h"

#define RX_BUFFER_SIZE      16u

/* Tipos de datos ------------------------------------------------------------*/

typedef struct{
	uint32_t baud_rate;
	word_length_t word_length;
	stop_bits_t stop_bits;
	parity_t parity;
}uart_handler_t;

uint32_t parser_word_length [] = { UART_WORDLENGTH_8B, UART_WORDLENGTH_9B };
uint32_t parser_stop_bits [] = { UART_STOPBITS_1, UART_STOPBITS_2 };
uint32_t parser_parity [] = { UART_PARITY_NONE, UART_PARITY_EVEN, UART_PARITY_ODD };

/* Variables ------------------------------------------------------------*/
UART_HandleTypeDef uart_handler;

uart_handler_t uart_handler_default = {
		/* baud_rate */		9600u,
		/* word_length */	UART_WORDLENGTH_8B,
		/* stop_bits */		UART_STOPBITS_1,
		/* parity */		UART_PARITY_NONE,
};

uint8_t rx_buffer[RX_BUFFER_SIZE] = {0u};

/* Funciones ------------------------------------------------------------*/

static bool configIsCorrect(uint32_t bauds, word_length_t word_bits, stop_bits_t stop_bits, parity_t parity)
{
	bool baud_rate_ok = (bauds >= 50u && bauds <= 921600u) ? true : false;
	bool word_length_ok = (word_bits >= 0u && word_bits <= 2u) ? true : false;
	bool stop_bits_ok = (stop_bits >= 0u && stop_bits <= 2u) ? true : false;
	bool parity_ok = (parity >= 0u && parity <= 3u) ? true : false;

	return (baud_rate_ok && word_length_ok && stop_bits_ok && parity_ok) ? true : false;
}

bool uartInit(uint32_t bauds, word_length_t word_bits, stop_bits_t stop_bits, parity_t parity)
{
	bool result = true;

	uart_handler.Instance        = USART3;

	if(true == configIsCorrect(bauds, word_bits, stop_bits, parity)){
		uart_handler.Init.BaudRate   = bauds;
		uart_handler.Init.WordLength = parser_word_length[word_bits];
		uart_handler.Init.StopBits   = parser_stop_bits[stop_bits];
		uart_handler.Init.Parity     = parser_parity[parity];
	}
	else{
		uart_handler.Init.BaudRate   = uart_handler_default.baud_rate;
		uart_handler.Init.WordLength = uart_handler_default.word_length;
		uart_handler.Init.StopBits   = uart_handler_default.stop_bits;
		uart_handler.Init.Parity     = uart_handler_default.parity;
	}

	uart_handler.Init.HwFlowCtl  	= UART_HWCONTROL_NONE;
	uart_handler.Init.Mode       	= UART_MODE_TX_RX;
	uart_handler.Init.OverSampling 	= UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&uart_handler) != HAL_OK){
		result = false;
	}

	return result;
}

void uartSendString(uint8_t *pstring, uint16_t size)
{
	HAL_UART_Transmit(&uart_handler, pstring, size, 0xFFFF);
}

void uartReceive(uint8_t *pstring, uint16_t size)
{
    HAL_UART_Receive(&uart_handler, pstring, size, 1000u);
}

void uartRead(uint8_t *buffer, uint8_t len)
{
    if((NULL != buffer) && (RX_BUFFER_SIZE < len)) {
        memcpy(buffer, rx_buffer, RX_BUFFER_SIZE);
    }
}

