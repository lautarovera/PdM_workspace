/**
 ******************************************************************************
 * @file    UART/UART_Printf/Src/main.c
 * @author  MCD Application Team
 * @brief   This example shows how to retarget the C library printf function
 *          to the UART.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DEBOUNCE_DELAY 			40u
#define DELAY_LED_1    			100u
#define DELAY_LED_2    			500u
#define DELAY_LED_3    			1000u
#define DELAY_ALT_LED_ALL    	500u
#define UART_BUFFER_SIZE		64u

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static delay_t Led1;
static delay_t Led2;
static delay_t Led3;

static uint8_t uart_buffer[UART_BUFFER_SIZE] = {0u};

/* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/* Callback for button pressed */
void buttonPressed(void)
{
	BSP_LED_Off(LED1);
	BSP_LED_Off(LED2);
	BSP_LED_Off(LED3);

	delayWrite(&Led1, DELAY_ALT_LED_ALL);
	delayWrite(&Led2, DELAY_ALT_LED_ALL);
	delayWrite(&Led3, DELAY_ALT_LED_ALL);

	sprintf((char *)uart_buffer, "\rLED 1: %d ms \tLED 2: %d ms \tLED 3: %d ms ", DELAY_ALT_LED_ALL, DELAY_ALT_LED_ALL, DELAY_ALT_LED_ALL);
}

/* Callback for button released */
void buttonReleased(void)
{
	BSP_LED_Off(LED1);
	BSP_LED_Off(LED2);
	BSP_LED_Off(LED3);

	delayWrite(&Led1, DELAY_LED_1);
	delayWrite(&Led2, DELAY_LED_2);
	delayWrite(&Led3, DELAY_LED_3);

	sprintf((char *)uart_buffer, "\rLED 1: %d ms \tLED 2: %d ms \tLED 3: %d ms ", DELAY_LED_1, DELAY_LED_2, DELAY_LED_3);
}

/* Initialize the UART buffer */
static void initUartBuffer(void)
{
	sprintf((char *)uart_buffer, "\rLED 1: %d ms \tLED 2: %d ms \tLED 3: %d ms ", DELAY_LED_1, DELAY_LED_2, DELAY_LED_3);
}

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	/* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Initialize BSP Led for LED1, LED2 and LED3 */
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);

	/* Initialize BSP PB for BUTTON_USER */
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Initialize API UART */
	/* Check API for input parameters, wrong inputs will trigger the default configuration */
	if(true != uartInit(9600u, 0u, 0u, 0u)){
		/* Initialization Error */
		Error_Handler();
	}

	/* Initialize API delay for LED1, LED2 and LED3 */
	delayInit(&Led1, DELAY_LED_1);
	delayInit(&Led2, DELAY_LED_2);
	delayInit(&Led3, DELAY_LED_3);

	/* Initialize Debounce API */
	debounceInit(DEBOUNCE_DELAY);

	/* Set pressed button callback */
	debounceSetPressedCbk(buttonPressed);

	/* Set released button callback */
	debounceSetReleasedCbk(buttonReleased);

	/* Initialize UART buffer */
	initUartBuffer();

	/* Infinite loop */
	while (true)
	{
		/* Debounce the button */
		debounceUpdate((bool_t)BSP_PB_GetState(BUTTON_USER));

		/* Toggle LEDs with non-blocking delays */
		if(delayRead(&Led1))
		{
			BSP_LED_Toggle(LED1);
		}

		if(delayRead(&Led2))
		{
			BSP_LED_Toggle(LED2);
		}

		if(delayRead(&Led3))
		{
			BSP_LED_Toggle(LED3);
		}

		uartSendString(uart_buffer, UART_BUFFER_SIZE);
	}
}


static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
