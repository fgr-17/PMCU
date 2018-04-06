/*
 * leds.c
 *
 *  Created on: Apr 6, 2018
 *      Author: piro2
 */

#include <stdint.h>


#include "sapi.h"        // <= Biblioteca sAPI

#include "antirreb.h"

#include "leds.h"

tick_t periodos[] = {50, 100, 1000};
tick_t periodoActivo = 50;
int periodoIndice = 0;


gpioMap_t leds[] = {LEDR, LEDG, LEDB, LED1, LED2, LED3};
gpioMap_t ledActivo = LEDR;
uint32_t ledIndice = 0;

void ActualizarFrecuenciaParpadeo();
void ActualizarLedActivo();

/**
 * @fn void ActualizarFrecuenciaParpadeo()
 *
 *
 */

void ActualizarFrecuenciaParpadeo()
{
	if (antirreb_tecla1.t == TECLA_PRESIONADA){
		periodoIndice++;
		if (periodoIndice >= PERIODOS_L)
			periodoIndice = 0;

		periodoActivo = periodos[periodoIndice];
	}
	return;
}

/**
 * @fn void ActualizarFrecuenciaParpadeo()
 *
 *
 */

void ActualizarLedActivo()
{
	if (antirreb_tecla2.t == TECLA_PRESIONADA){
		ledIndice++;
		if (ledIndice >= LEDS_L)
			ledIndice = 0;

		gpioWrite(ledActivo, 0);	// apago el led anterior
		ledActivo = leds[ledIndice];


		antirreb_tecla2.t = TECLA_SUELTA;
	}
	return;
}
