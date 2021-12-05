/**
 * @file main.c
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */


/*----------------------- INCLUDES -----------------------*/
#include "main.h"
#include "API_delay.h"
#include "API_uart.h"
#include "CmdMng.h"
#include "BtlCore.h"

/*----------------------- DEFINES ------------------------*/

/*----------------------- TYPES --------------------------*/

/*----------------------- VARIABLES ----------------------*/

/*----------------------- PROTOTYPES ---------------------*/
static void Init(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Reset(void);

/*----------------------- ROUTINES -----------------------*/
int main()
{
    Init();

    for(;;)
    {
        if(BTLCORE_BOOLEAN_TRUE == BtlCore_ResetRequested()) {
            Reset();
        }

        CmdMng_Task();
        BtlCore_Task();
    }
}

static void Init(void)
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

    /* Initialize API UART */
    /* Check API for input parameters, wrong inputs will trigger the default configuration */
    if(true != uartInit(921600u, 0u, 0u, 0u)){
        /* Initialization Error */
        Error_Handler();
    }

    CmdMng_Init();
    BtlCore_Init();

//    /* Check if the system has resumed from WWDG reset */
//    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET) {
//        /* WWDGRST flag set: Turn LED1 on */
//        BSP_LED_On (LED1);
//
//        /* Clear reset flags */
//        __HAL_RCC_CLEAR_RESET_FLAGS();
//    }
//    else {
//        /* WWDGRST flag is not set: Turn LED1 off */
//        BSP_LED_Off (LED1);
//    }
//
//    /* Configure the WWDG peripheral */
//    /* WWDG clock counter = (PCLK1 (45MHz)/4096)/8) = 1373 Hz (~728 us)
//     WWDG Window value = 80 means that the WWDG counter should be refreshed only
//     when the counter is below 80 (and greater than 64/0x40) otherwise a reset will
//     be generated.
//     WWDG Counter value = 127, WWDG timeout = ~728 us * 64 = 46 ms */
//    WwdgHandle.Instance = WWDG;
//
//    WwdgHandle.Init.Prescaler = WWDG_PRESCALER_8;
//    WwdgHandle.Init.Window = 80;
//    WwdgHandle.Init.Counter = 127;
//
//    if (HAL_WWDG_Init (&WwdgHandle) != HAL_OK) {
//        /* Initialization Error */
//        Error_Handler ();
//    }
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

static void Reset(void)
{
    HAL_NVIC_SystemReset();
}
/* EOF */
