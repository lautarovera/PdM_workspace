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
}

void debounceSetReleasedCbk(void (*callback)())
{
	if(NULL != callback) {
		buttonReleasedCbk = callback;
	}
}

void debounceUpdate(bool_t buttonState)
{
	switch(currentState)
	{
		case BUTTON_UP:
			if(true == buttonState) {
				delayRead(&debounceDelay);
				currentState = BUTTON_FALLING;
			}
			break;
		case BUTTON_FALLING:
			if(true == delayRead(&debounceDelay)) {
				if(true == buttonState) {
					(*buttonPressedCbk)();
					currentState = BUTTON_DOWN;
				}
			}
			else {
				currentState = BUTTON_UP;
			}
			break;
		case BUTTON_DOWN:
			if(false == buttonState) {
				delayRead(&debounceDelay);
				currentState = BUTTON_RISING;
			}
			break;
		case BUTTON_RISING:
			if(true == delayRead(&debounceDelay)) {
				if(false == buttonState) {
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
