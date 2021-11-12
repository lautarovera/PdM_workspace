/*
 * API_delay.c
 *
 *  Created on: Nov 5, 2021
 *      Author: lautaro
 */

#include "API_delay.h"

bool_t delayInit(delay_t * delay, tick_t duration)
{
	bool_t result = false;

	if((delay != NULL) && (duration <= MAX_DELAY))
	{
		delay->startTime = 0U;
		delay->duration = duration;
		delay->running = false;

		result = true;
	}

	return result;
}

bool_t delayRead(delay_t * delay)
{
	bool_t result = false;

	if(delay != NULL)
	{
		if(true != delay->running)
		{
			delay->running = true;
			delay->startTime = (tick_t)HAL_GetTick();
		}

		if(((tick_t)HAL_GetTick() - delay->startTime) >= delay->duration)
		{
			delay->running = false;
			result = true;
		}
	}

	return result;
}

void delayWrite(delay_t * delay, tick_t duration)
{
	if((delay != NULL) && (duration <= MAX_DELAY))
	{
		delay->duration = duration;
	}
}
