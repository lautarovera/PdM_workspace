/*
 * API_debounce.c
 *
 *  Created on: Nov 5, 2021
 *      Author: lautaro
 */

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include "API_debounce.h"
#include "API_delay.h"

#define DEBOUNCE_DELAY 40u

/* Estructura para el manejo de la API */
typedef enum{
   BUTTON_UP,
   BUTTON_FALLING,
   BUTTON_DOWN,
   BUTTON_RISING
} debounce_t;

static debounce_t currentState;

static delay_t debounceDelay;

void (*buttonPressedCbk)(void);

void (*buttonReleasedCbk)(void);

bool_t debounceInit(void)
{
	bool_t result = delayInit(&debounceDelay, DEBOUNCE_DELAY);
	currentState = BUTTON_UP;

	return result;
}

void debounceSetPressedCbk(void (*callback)())
{
	if(NULL != callback) {
		buttonPressedCbk = callback;
	}
	else {
		BSP_LED_Toggle(LED1);
	}
}

void debounceSetReleasedCbk(void (*callback)())
{
	if(NULL != callback) {
		buttonReleasedCbk = callback;
	}
	else {
		BSP_LED_Toggle(LED2);
	}
}

void debounceUpdate(void)
{
	switch(currentState)
	{
		case BUTTON_UP:
			if(true == (bool_t)BSP_PB_GetState(BUTTON_USER)) {
				delayRead(&debounceDelay);
				currentState = BUTTON_FALLING;
			}
			break;
		case BUTTON_FALLING:
			if(true == delayRead(&debounceDelay)) {
				if(true == (bool_t)BSP_PB_GetState(BUTTON_USER)) {
					(*buttonPressedCbk)();
					currentState = BUTTON_DOWN;
				}
			}
			else {
				currentState = BUTTON_UP;
			}
			break;
		case BUTTON_DOWN:
			if(false == (bool_t)BSP_PB_GetState(BUTTON_USER)) {
				delayRead(&debounceDelay);
				currentState = BUTTON_RISING;
			}
			break;
		case BUTTON_RISING:
			if(true == delayRead(&debounceDelay)) {
				if(false == (bool_t)BSP_PB_GetState(BUTTON_USER)) {
					(*buttonReleasedCbk)();
					currentState = BUTTON_UP;
				}
			}
			else {
				currentState = BUTTON_DOWN;
			}
			break;
		default:
			break;
	}
}
