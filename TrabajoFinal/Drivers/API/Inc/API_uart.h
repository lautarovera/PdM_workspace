/*
 * API_uart.h
 *
 *  Created on: Nov 22, 2021
 *      Author: lautaro
 */

#ifndef API_UART_H_
#define API_UART_H_

#include <stdint.h>
#include <stdbool.h>

/* Defines ------------------------------------------------------------*/

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

/* Variables ------------------------------------------------------------*/

/* Prototypes ------------------------------------------------------------*/

/**
 * @brief Initialize the UART component
 * @param bauds Input for the baud rate
 * @param word_bits Input for the word length
 * @param stop_bits Input for stop bits
 * @param parity Input for parity bits
 * @return true if configuration was successful, otherwise false
 */
bool uartInit(uint32_t bauds, word_length_t word_bits, stop_bits_t stop_bits, parity_t parity);

/**
 * @brief Send the input string through UART
 * @param pstring Pointer to string
 * @param size Size of the string
 */
void uartSendString(uint8_t *pstring, uint16_t size);

void uartReceive(uint8_t *pstring, uint16_t size);

void uartRead(uint8_t *buffer, uint8_t len);

#endif /* API_UART_H_ */
