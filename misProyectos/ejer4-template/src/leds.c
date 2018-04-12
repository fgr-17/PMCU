/*
 * leds.c
 *
 *  Created on: Apr 6, 2018
 *      Author: piro2
 */


/* =========================================================================================
 * 					INCLUSION DE ARCHIVOS
 * =========================================================================================
 */

#include <stdint.h>
#include "cooperativeOs_isr.h"       // <= dispatcher and task management header

#include "cooperativeOs_scheduler.h" // <= scheduler and system initialization header
#include "sapi.h"        // <= Biblioteca sAPI
//#include "antirreb.h"
#include "colas_datos.h"

#include "uart.h"

#include "leds.h"


/* =========================================================================================
 * 					VARIABLES GLOBALES
 * =========================================================================================
 */

tick_t periodos[] = {50, 100, 1000};
tick_t periodoActivo = 50;
int periodoIndice = 0;


static bool_t ledState = OFF;

gpioMap_t leds[] = {LEDR, LEDG, LEDB, LED1, LED2, LED3};
gpioMap_t ledActivo = LEDR;
uint32_t ledIndice = 0;

int32_t tareaBlinkyID;


/* =========================================================================================
 * 					PROTOTIPO DE FCS
 * =========================================================================================
 */

void taskActualizarLeds();
void taskBlinkLed(void);

/* =========================================================================================
 * 					IMPLEMENTACION DE FCS
 * =========================================================================================
 */

/**
 * @fn void taskBlinkLed(void)
 *
 * @brief Tarea de parpadeo de led.
 *
 */

void taskBlinkLed(void){

   if (ledState == ON){
	  ledState = OFF; // Apago el pin
   }
   else{
	  ledState = ON; // Prendo el pin
   }

   // Use of digitalWrite
   // gpioWrite( LED3, ledState );
   gpioToggle( ledActivo );
}

/**
 * @fn void ActualizarFrecuenciaParpadeo()
 *
 *
 */

void taskActualizarLeds(void)
{
	volatile uint8_t datoLeido;
	volatile int8_t estadoColaRx;
	estadoColaRx = LeerCola(&colaRx, &datoLeido);
	//if (antirreb_tecla1.t == TECLA_PRESIONADA){
	if (estadoColaRx == LEER_COLA_COLA_VACIA)
		return;

	else {

		switch(datoLeido) {

		case 't':
		case 'T':
			periodoIndice++;
			if (periodoIndice >= PERIODOS_L)
				periodoIndice = 0;

			periodoActivo = periodos[periodoIndice];
			// reiniciar tarea blinky con nuevo periodo

			EscribirCadenaCola ("Periodo actualizado\n", &colaTx);
			schedulerUpdatePeriod(tareaBlinkyID, periodoActivo);
			break;

		case 'L':
		case 'l':

			ledIndice++;
			if (ledIndice >= LEDS_L)
				ledIndice = 0;

			gpioWrite(ledActivo, 0);	// apago el led anterior
			ledActivo = leds[ledIndice];
			EscribirCadenaCola ("Led activo actualizado\n", &colaTx);

			break;

		default:
			EscribirCadenaCola ("Comando no reconocido\n", &colaTx);

		}
	}
	return;
}

