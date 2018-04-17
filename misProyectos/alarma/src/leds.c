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

int32_t tareaBlinkyID;


/* =========================================================================================
 * 					PROTOTIPO DE FCS
 * =========================================================================================
 */

void taskActualizarLeds();
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

	switch(alarmaEstado) {

	case DESARMADA:
		gpioWrite(ALARMA_LED_DESARMADA, ON);
		break;
	case ARMADA:
		gpioWrite(ALARMA_LED_DESARMADA, ON);
		break;
	case DISPARADA:
		if (ledState == ON){
		  ledState = OFF; // Apago el pin
	   }
	   else{
		  ledState = ON; // Prendo el pin
	   }
	   gpioToggle(ALARMA_LED_DISPARADA );
		break;
	case ESPERANDO_PASS:
		gpioWrite(ALARMA_LED_ESPERANDO_PASS, ON);
		break;
	case USUARIO_SALIENDO:
		gpioWrite(ALARMA_LED_DESARMADA, ON);
		break;
	case USUARIO_ENTRANDO:
		gpioWrite(ALARMA_LED_DESARMADA, ON);
		break;
   default:
      apagarLeds();

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
 * @fn void ActualizarFrecuenciaParpadeo()
 *
 *
 */

void taskActualizarLeds(void)
{

	volatile uint8_t periodoAscii[DELAY_L];
	volatile uint32_t nCifras = 0, i_cifra, periodoEntero = 0;

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

			/* En caso que mande solo la letra 't' */
			if(LeerCola(&colaRx, periodoAscii) == LEER_COLA_COLA_VACIA)
			{

				periodoIndice++;
				if (periodoIndice >= PERIODOS_L)
					periodoIndice = 0;

				periodoActivo = periodos[periodoIndice];
				// reiniciar tarea blinky con nuevo periodo

				EscribirCadenaCola ("Periodo actualizado\n\r", &colaTx);
				schedulerUpdatePeriod(tareaBlinkyID, periodoActivo);
			}
			/* En caso que mande la letra 'txxxx' */

			else{

				nCifras = 1;

				while(LeerCola(&colaRx, &periodoAscii[nCifras]) != LEER_COLA_COLA_VACIA){

					if(periodoAscii[nCifras] < '0' || periodoAscii[nCifras] > '9')
					{
						/* vacio la cola de datos */
						while(LeerCola(&colaRx, &periodoAscii[nCifras]) != LEER_COLA_COLA_VACIA);
						/* mando msj de error */
						EscribirCadenaCola ("ERROR: el delay especificado no es un numero \n\r", &colaTx);
						return;
					}

					nCifras++;
					if(nCifras >= DELAY_L){
						/* vacio la cola de datos */
						while(LeerCola(&colaRx, &periodoAscii[nCifras]) != LEER_COLA_COLA_VACIA);
						/* mando msj de error */
						EscribirCadenaCola ("ERROR: Tiempo maximo 1000ms \n\r", &colaTx);
						/* salgo */
						return;
						// chequear error de mas de 4 cifras
					}
				}

				periodoEntero = AsciiAEntero(periodoAscii, nCifras);

				periodoActivo = periodoEntero;
				EscribirCadenaCola ("Periodo actualizado\n\r", &colaTx);
				schedulerUpdatePeriod(tareaBlinkyID, periodoActivo);
			}
			break;

		case 'L':
		case 'l':

			ledIndice++;
			if (ledIndice >= LEDS_L)
				ledIndice = 0;

			gpioWrite(ledActivo, 0);	// apago el led anterior
			ledActivo = leds[ledIndice];
			EscribirCadenaCola ("Led activo actualizado\n\r", &colaTx);

			break;

		default:
			EscribirCadenaCola ("Comando no reconocido\n\r", &colaTx);

		}
	}
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
