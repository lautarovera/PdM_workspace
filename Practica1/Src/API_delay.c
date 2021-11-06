/*
 * API_delay.c
 *
 *  Created on: Nov 5, 2021
 *      Author: lautaro
 */

#include "API_delay.h"

void delayInit( delay_t * delay, tick_t duration )
{
	if( delay != NULL )
	{
		delay->startTime = 0U;
		delay->duration = duration;
		delay->running = false;
	}
}

bool_t delayRead( delay_t * delay )
{
	bool_t result = false;

	if( delay != NULL )
	{
		if( true != delay->running )
		{
			delay->running = true;
			delay->startTime = (tick_t)HAL_GetTick();
		}

		result = (((tick_t)HAL_GetTick() - delay->startTime) >= delay->duration) ? true : false;
	}

	return result;
}

void delayWrite( delay_t * delay, tick_t duration )
{
	if( delay != NULL )
	{
		delay->duration = duration;
	}
}
