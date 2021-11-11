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

/* Redefinición de bool para coherencia de estilos */
typedef bool bool_t;

/* Inicializa la API */
bool_t debounceInit(void);

/* Configura el callback para el evento del botón presionado
 * Si se pasa NULL como parámetro se ejecuta el callback por defecto
 * El callback por defecto es "togglear" el LED 1
 */
void debounceSetPressedCbk(void (*callback)());

/* Configura el callback para el evento del botón liberado
 * Si se pasa NULL como parámetro se ejecuta el callback por defecto
 * El callback por defecto es "togglear" el LED 2
 */
void debounceSetReleasedCbk(void (*callback)());

/* Actualiza la MEF */
void debounceUpdate(bool_t buttonState);

#endif /* API_DELAY_H_ */
