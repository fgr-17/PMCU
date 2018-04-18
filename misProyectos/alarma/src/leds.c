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

#include "alarma.h"

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

/* =========================================================================================
 * 					PROTOTIPO DE FCS
 * =========================================================================================
 */

void taskBlinkLed(void);
void apagarLeds (void);
static int32_t AsciiAEntero (uint8_t*cadena, int32_t nCifras);

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

	int32_t estadoNuevo;

	if(alarmaEstado == DISPARADA) {

		if (ledState == ON){
		  ledState = OFF; // Apago el pin
		}
		else{
		  ledState = ON; // Prendo el pin
		}
		gpioToggle(ALARMA_LED_DISPARADA);

	}
}

/**
 * @fn apagarLeds
 *
 * @brief apago todos los leds
 */

void apagarLeds (void)
{
   gpioWrite(ALARMA_LED_DESARMADA, ON);


   return;
}

/**
 * @fn int32_t AsciiAEntero (uint8_t*cadena, int32_t nCifras)
 *
 * @ convierto string a cadena
 *
 */

int32_t AsciiAEntero (uint8_t*cadena, int32_t nCifras)
{
	int32_t resultado = 0, i_cifra;

	for(i_cifra = 0; i_cifra < nCifras; i_cifra++)
	{
		resultado *= 10;
		resultado += cadena[i_cifra] - '0';
	}


	return resultado;

}
