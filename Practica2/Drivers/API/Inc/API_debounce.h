/*
 * API_debounce.h
 *
 *  Created on: Nov 5, 2021
 *      Author: lautaro
 */

#ifndef API_DEBOUNCE_H_
#define API_DEBOUNCE_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

/* Inicializa la API */
void debounceInit(void);

/* Configura el callback para el evento del botón presionado */
void debounceSetPressedCbk(void (* callback)());

/* Configura el callback para el evento del botón liberado */
void debounceSetReleasedCbk(void (* callback)());

/* Actualiza la MEF */
void debounceUpdate(bool_t buttonState);

#endif /* API_DELAY_H_ */
