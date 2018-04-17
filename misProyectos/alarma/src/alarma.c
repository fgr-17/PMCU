/**
 * alarma.c
 *
 *  Created on: 16/4/2018
 *      Author: piro2
 */

/* =================================== [inclusion de archivos] ============================= */
#include <stdint.h>
#include <string.h>
#include "sapi.h"

#include "colas_datos.h"

#include "alarma.h"
#include "antirreb.h"

/* =================================== [funciones generales] ============================= */

static int32_t lineaColaAString (int8_t*string, int32_t largo, t_cola cola);

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

/** @brief timeout para cuentas de tiempo */
timeout_t tout1;


/**
 * @fn inicializarTaskAlarmaMEF
 *
 * @brief Inicializacion de la maquina de estados
 *
 */

void inicializarTaskAlarmaMEF (void){

	alarmaEstado = DESARMADA;
	inicializarTimeout(tout1);

	return;
}

/**
 *
 * @fn void taskAlarmaMEF (void)
 *
 * @brief maquina de estados ppal de manejo de la alarma.
 *
 */

/** @brief tiempo que espero si el usuario quiso armar la alarma con un pass incorrecto */
#define ALARMA_TIMEOUT_PASSWORD_ARMAR_INCORRECTO_MS			20000
/** @brief string con el password. Deberia estar guardado en la flash y poder cambiarse desde un menu tecnico */
#define ALARMA_STRING_PASSWORD		"1234"
/** @brief string para avisar que voy a salir */
#define ALARMA_STRING_SALIR			"salgo"

void taskAlarmaMEF (void)
{
	uint8_t cadena [CADENA_L];

	switch(alarmaEstado) {

	case DESARMADA:

		if(colaRx.datosNuevos){

			colaRx.datosNuevos = 0;								// bajo el flag de datos nuevos
			lineaColaAString(cadena, CADENA_L, colaRx);			// saco linea de la cola de datos a un string
			if(strcmp(cadena, ALARMA_STRING_SALIR)) {

			}
			else
			{
				// Recibi un comando incorrecto
				EscribirCadenaCola ("Comando incorrecto\n\r", &colaTx);
				alarmaEstado = DESARMADA;
			}

		}
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
 *
 * @fn inicializarTimeout (void)
 *
 * @brief inicializo estructura de datos
 *
 */

void inicializarTimeout (timeout_t*tout) {

	tout->activo = FALSE;
	tout->cuenta = 0;
	tout->maximo = 0;
	tout->estado = INACTIVO;
	return;
}

/**
 *
 * @fn activarTimeout (void)
 *
 * @brief tarea para hacer timeouts
 *
 */

void activarTimeout (timeout_t*tout, uint32_t timeout_ms)
{
	uint32_t cuenta_max;

	cuenta_max = timeout_ms / TIMEOUT_PERIODO;

	tout->cuenta = 0;
	tout->maximo = cuenta_max;

	tout->activo = TRUE;

	return;
}

/**
 *
 * @fn taskTimeout (void)
 *
 * @brief tarea para hacer timeouts
 *
 */

void taskTimeout (timeout_t*tout) {

	if(tout->activo == FALSE)			// si el timeout esta inactivo, salgo
		return;

	if(tout->activo == TRUE){			// si el timeout esta activo, empiezo a contar
		tout->estado = CONTANDO;

		tout->cuenta++;

		if(tout->cuenta >= tout->maximo){
			tout->activo = FALSE;
			tout->cuenta = 0;
			tout->estado = FINALIZO;
		}

	}
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
 * @fn bool sensorVentanaActivada (void)
 *
 * @brief chequeo si alguno de los sensores de ventana se activo
 *
 */

bool_t sensorVentanaActivada (void) {

	if(antirreboteMEF_ventana1.t == TECLA_PRESIONADA){
		antirreboteMEF_ventana1.t = TECLA_SUELTA;
		return TRUE;
	}
	if(antirreboteMEF_ventana2.t == TECLA_PRESIONADA){
			antirreboteMEF_ventana2.t = TECLA_SUELTA;
			return TRUE;
	}
	if(antirreboteMEF_ventana2.t == TECLA_PRESIONADA){
			antirreboteMEF_ventana2.t = TECLA_SUELTA;
			return TRUE;
	}


}

/**
 * @fn colaAString (int8_t*string, cola_t cola)
 *
 * @brief copio una linea de una cola de datos a un string
 */

int32_t lineaColaAString (int8_t*string, int32_t largo, t_cola cola) {

	int32_t i;

	for(i = 0; i < largo; i++) {

		if(LeerCola(&cola, &string[i]) != LEER_COLA_COLA_VACIA)
			return 1;			// salgo porque la cola de datos esta vacia

		if(string[i] == '\n') {
			string[i] = '\0';	// cambio el enter por un fin de cadena
			return 0;			// salgo porque llegue al final de cadena
		}

	}

	return 2;					// llegue al final de la cadena y siguen quedando datos o no llegue a un enter.


}
