/*
 * API_debounce.c
 *
 *  Created on: Nov 5, 2021
 *      Author: lautaro
 */

#include "API_debounce.h"
#include "API_delay.h"

#define DEBOUNCE_DELAY 40u

/* Estructura para el manejo de la API */
typedef enum{
   BUTTON_UP,
   BUTTON_FALLING,
   BUTTON_DOWN,
   BUTTON_RISING
} button_state_t;

static button_state_t currentState;

static delay_t debounceDelay;

void (* buttonPressedCbk)();

void (* buttonReleasedCbk)();

void debounceInit(void)
{
	currentState = BUTTON_UP;
	delayInit(&debounceDelay, DEBOUNCE_DELAY);
}

void debounceSetPressedCbk(void (* callback)())
{
	(* buttonPressedCbk)() = &callback();
}

void debounceSetReleasedCbk(void (* callback)())
{
	(* buttonReleasedCbk)() = &callback();
}

void debounceUpdate(bool_t buttonState)
{
	switch(currentState)
	{
		case BUTTON_UP:
			if(!buttonState) {
				delayRead(&debounceDelay);
				currentState = BUTTON_FALLING;
			}
			break;
		case BUTTON_FALLING:
			if(delayRead(&debounceDelay)) {
				(* buttonPressedCbk)();
				currentState = BUTTON_DOWN;
			}
			break;
		case BUTTON_DOWN:
			if(buttonState) {
				delayRead(&debounceDelay);
				currentState = BUTTON_FALLING;
			}
			break;
		case BUTTON_RISING:
			if(delayRead(&debounceDelay)) {
				(* buttonReleasedCbk)();
				currentState = BUTTON_UP;
			}
			break;
		default:
			break;
	}
}
