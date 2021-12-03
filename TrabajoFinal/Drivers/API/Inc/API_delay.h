/*
 * API_delay.h
 *
 *  Created on: Nov 5, 2021
 *      Author: lautaro
 */

#ifndef API_DELAY_H_
#define API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

/* Maximum value for delay */
#define MAX_DELAY 10000u

/* Variable que define el tiempo */
typedef uint32_t tick_t;

/* Estructura para el manejo de la API */
typedef struct{
   tick_t startTime;	/* Marca de tiempo de inicio */
   tick_t duration;		/* Duración del delay */
   bool running;		/* Bandera para indicar que el delay esta corriendo */
} delay_t;

/* Inicializa la API */
bool delayInit(delay_t * delay, tick_t duration);
/* Indica si el delay esta corriendo o no */
bool delayRead(delay_t * delay);
/* Define la duración del delay */
void delayWrite(delay_t * delay, tick_t duration);



#endif /* API_DELAY_H_ */
