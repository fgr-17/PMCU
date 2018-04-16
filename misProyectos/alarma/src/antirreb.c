/*
 * antirreb.c
 *
 *  Created on: Apr 6, 2018
 *      Author: piro2
 */

#include <stdint.h>
#include "sapi.h"        // <= Biblioteca sAPI

#include <antirreb.h>


antirreb_t antirreb_tecla1;
antirreb_t antirreb_tecla2;



/**
 * @fn void Inicializar_fsmAntirrebote2 (void)
 *
 */

void Inicializar_fsmAntirrebote (antirreb_t*antirreb, gpioMap_t tecla_asigada ){

	antirreb->estado = BUTTON_UP;
	antirreb->t = TECLA_SUELTA;
	antirreb->tiempo_ventana = ANTIRREBOTE_DELAY;
	antirreb->tecla_pin = tecla_asigada;
}

/**
 *
 * @fn void fsmAntirrebote (void)
 *
 *
 *
 */

void fsmAntirrebote (antirreb_t*antirreb){

	bool_t tecValue = TECLA_ARRIBA;


	switch(antirreb->estado)
	{

	case BUTTON_UP:
		tecValue = gpioRead( antirreb->tecla_pin );
		if(tecValue == TECLA_ARRIBA){
			antirreb->estado = BUTTON_UP;
		}
		else if (tecValue == TECLA_ABAJO)
		{
			antirreb->estado = BUTTON_FALLING;
			delayConfig(&antirreb->delay, antirreb->tiempo_ventana);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreb->delay);
		}

		break;
	case BUTTON_FALLING:

		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreb->delay)){
			tecValue = gpioRead( antirreb->tecla_pin );
			if(tecValue == TECLA_ARRIBA){
				antirreb->estado = BUTTON_UP;
			}
			else if(tecValue == TECLA_ABAJO){
				antirreb->estado = BUTTON_DOWN;
				antirreb->t = TECLA_PRESIONADA;
			}

		}
		else{
			antirreb->estado = BUTTON_FALLING;
		}
		break;
	case  BUTTON_DOWN:

		tecValue = gpioRead( antirreb->tecla_pin );
		if(tecValue == TECLA_ABAJO){
			antirreb->estado = BUTTON_DOWN;
		}
		else if (tecValue == TECLA_ARRIBA)
		{
			antirreb->estado = BUTTON_RAISING;
			delayConfig(&antirreb->delay, antirreb->tiempo_ventana);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreb->delay);
		}
		break;

	case BUTTON_RAISING:


		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreb->delay)){
			tecValue = gpioRead( antirreb->tecla_pin );
			if(tecValue == TECLA_ABAJO){
				antirreb->estado = BUTTON_DOWN;
			}
			else if(tecValue == TECLA_ARRIBA){
				antirreb->estado = BUTTON_UP;
			}

		}
		else{
			antirreb->estado = BUTTON_RAISING;
		}
		break;

	default:
		Inicializar_fsmAntirrebote(antirreb, antirreb->tecla_pin);

	}

	return;
}



