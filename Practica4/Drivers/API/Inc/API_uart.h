/*
 * API_uart.h
 *
 *  Created on: Nov 22, 2021
 *      Author: lautaro
 */

#ifndef API_UART_H_
#define API_UART_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Defines ------------------------------------------------------------*/

#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3

/* Types ------------------------------------------------------------*/

typedef enum{
	API_UART_WL_8B,
	API_UART_WL_9B
}word_length_t;

typedef enum{
	API_UART_SB_1B,
	API_UART_SB_2B
}stop_bits_t;

typedef enum{
	API_UART_P_NONE,
	API_UART_P_EVEN,
	API_UART_P_ODD
}parity_t;

/* Prototypes ------------------------------------------------------------*/

bool uartInit(uint32_t bauds, word_length_t word_bits, stop_bits_t stop_bits, parity_t parity);

void uartSendString(uint8_t *pstring, uint16_t size);

#endif /* API_UART_H_ */
