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

/* Inicializa la API definiendo el tiempo para el antirrebote */
bool debounceInit(uint16_t debounce_delay);

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
void debounceUpdate(bool buttonState);

#endif /* API_DELAY_H_ */
