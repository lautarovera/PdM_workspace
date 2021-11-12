/*
 * API_delay.h
 *
 *  Created on: Nov 5, 2021
 *      Author: lautaro
 */

#ifndef API_DELAY_H_
#define API_DELAY_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

/* Maximum value for delay */
#define MAX_DELAY 10000u

/* Variable que define el tiempo */
typedef uint32_t tick_t;

/* Redefinición de bool para coherencia de estilos */
typedef bool bool_t;

/* Estructura para el manejo de la API */
typedef struct{
   tick_t startTime;	/* Marca de tiempo de inicio */
   tick_t duration;		/* Duración del delay */
   bool_t running;		/* Bandera para indicar que el delay esta corriendo */
}delay_t;

/* Inicializa la API */
void delayInit(delay_t * delay, tick_t duration);
/* Indica si el delay esta corriendo o no */
bool_t delayRead(delay_t * delay);
/* Define la duración del delay */
void delayWrite(delay_t * delay, tick_t duration);



#endif /* API_DELAY_H_ */
