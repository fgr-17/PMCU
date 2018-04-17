/**
 * alarma.c
 *
 *  Created on: 16/4/2018
 *      Author: piro2
 */

/* =================================== [inclusion de archivos] ============================= */
#include <stdint.h>

#include "alarma.h"
#include "antirreb.h"

/* =================================== [variables globales] ============================= */

/** @brief variable de estado de la MEF de alarma */
alarma_t alarmaEstado;

/** @brief antirrebote del sensor de puerta */
antirreb_t antirreboteMEF_puerta;
/** @brief antirrebote del sensor de ventana 1 */
antirreb_t antirreboteMEF_ventana1;
/** @brief antirrebote del sensor de ventana 2 */
antirreb_t antirreboteMEF_ventana2;
/** @brief antirrebote del sensor de ventana 3 */
antirreb_t antirreboteMEF_ventana3;




/**
 * @fn inicializarTaskAlarmaMEF
 *
 * @brief Inicializacion de la maquina de estados
 *
 */

void inicializarTaskAlarmaMEF (void){

	alarmaEstado = DESARMADA;

	return;
}

/**
 *
 * @fn void taskAlarmaMEF (void)
 *
 * @brief maquina de estados ppal de manejo de la alarma.
 *
 */
void taskAlarmaMEF (void)
{

	switch(alarmaEstado) {

	case DESARMADA:


		break;
	case ARMADA:
		break;
	case DISPARADA:
		break;
	case ESPERANDO_PASS:
		break;
	case USUARIO_SALIENDO:
		break;
	case USUARIO_ENTRANDO:
		break;
	default:
		inicializarTaskAlarmaMEF();

	}


	return;
}

/**
 * @fn inicializarSensores
 *
 * @brief inicializo los sensores de alarma
 *
 */

void inicializarSensores (void) {

	inicializarAntirreboteMEF(antirreboteMEF_puerta, ALARMA_SENSOR_PUERTA);
	inicializarAntirreboteMEF(antirreboteMEF_ventana1, ALARMA_SENSOR_VENTANA1);
	inicializarAntirreboteMEF(antirreboteMEF_ventana2, ALARMA_SENSOR_VENTANA2);
	inicializarAntirreboteMEF(antirreboteMEF_ventana3, ALARMA_SENSOR_VENTANA3);
	return;
}


/**
 * @fn taskLeerSensores
 *
 * @brief levanto el estado de todas los sensores (teclas)
 *
 */

void taskLeerSensores (void) {

	antirreboteMEF(&antirreboteMEF_puerta);
	antirreboteMEF(&antirreboteMEF_ventana1);
	antirreboteMEF(&antirreboteMEF_ventana2);
	antirreboteMEF(&antirreboteMEF_ventana3);
	return;
}

/**
 * @fn bool sensorVentanaActivada (void)
 *
 * @brief chequeo si alguno de los sensores de ventana se activo
 *
 */

bool_t sensorVentanaActivada (void) {

	if(antirreboteMEF_ventana1.t == TECLA_PRESIONADA){
		antirreboteMEF_ventana1.t = TECLA_SUELTA
		return TRUE;
	}
	if(antirreboteMEF_ventana2.t == TECLA_PRESIONADA){
			antirreboteMEF_ventana2.t = TECLA_SUELTA
			return TRUE;
	}
	if(antirreboteMEF_ventana2.t == TECLA_PRESIONADA){
			antirreboteMEF_ventana2.t = TECLA_SUELTA
			return TRUE;
	}


}
