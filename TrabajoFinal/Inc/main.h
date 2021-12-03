/**
 * @file main.h
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */

#ifndef MAIN_H_
#define MAIN_H_

/*----------------------- INCLUDES -----------------------*/
#include "stm32f4xx_hal.h"          /* <- HAL include */
#include "stm32f4xx_nucleo_144.h"   /* <- BSP include */

/*----------------------- DEFINES ------------------------*/
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

/*----------------------- TYPES --------------------------*/

/*----------------------- VARIABLES ----------------------*/

/*----------------------- PROTOTYPES ---------------------*/

#endif /* MAIN_H_ */
