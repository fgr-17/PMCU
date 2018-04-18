/**
 * alarma.c
 *
 *  Created on: 16/4/2018
 *      Author: piro2
 */

/* =================================== [inclusion de archivos] ============================= */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sapi.h"

#include "colas_datos.h"

#include "alarma.h"
#include "antirreb.h"

/* =================================== [funciones generales] ============================= */

static void mensajeAlarma (int8_t * mensaje);
static bool_t sensorVentanaActivado (void);
static bool_t sensorPuertaActivado (void);
static void inicializarTimeout (timeout_t*tout);
static void activarTimeout (timeout_t*tout, uint32_t timeout_ms);

void inicializarSensores (void);
void taskLeerSensores (void);
void taskTimeout (timeout_t*tout);
void taskAlarmaMEF (void);
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
	inicializarTimeout(&tout1);

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
	uint8_t cadena [CADENA_L];

	static uint8_t cantidadIntentosClaveMal = 0;

	switch(alarmaEstado) {

	case DESARMADA:

		if(colaRx.datosNuevos){												// pregunto si tengo datos nuevos
			colaRx.datosNuevos = 0;											// bajo el flag de datos nuevos
			lineaColaAString(cadena, CADENA_L, colaRx);						// saco linea de la cola de datos a un string
			if(strcmp(cadena, ALARMA_STRING_SALIR)) {

				mensajeAlarma("Ingrese la contraseña:");
				alarmaEstado = ESPERANDO_PASS;
			}
			else
			{
				// Recibi un comando incorrecto
				mensajeAlarma ("Comando incorrecto");						// mando mensaje al display
				activarTimeout(&tout1, ALARMA_TIMEOUT_ESPERANDO_PASS_MS);	// empiezo a contar timeout
				alarmaEstado = DESARMADA;									// cambio de estado
			}
		}
		break;
	case ESPERANDO_PASS:

		if(colaRx.datosNuevos){												// pregunto si tengo datos nuevos
			colaRx.datosNuevos = 0;											// bajo el flag de datos nuevos
			lineaColaAString(cadena, CADENA_L, colaRx);						// saco linea de la cola de datos a un string

			if(strcmp(cadena, ALARMA_STRING_PASSWORD)) {
				mensajeAlarma("Alarma armada.\nTiene 60s para salir.");
				activarTimeout(&tout1, ALARMA_TIMEOUT_SALIENDO_MS);			// empiezo a contar timeout para salir
				alarmaEstado = USUARIO_SALIENDO;
			}
			else
			{
				// Recibi un comando incorrecto
				mensajeAlarma ("Contraseña incorrecta");					// mando mensaje al display
				cantidadIntentosClaveMal++;									// incremento el contador de clave erronea

				if(cantidadIntentosClaveMal >= ALARMA_CANTIDAD_INTENTOS_CLAVE_MAL_MAX) {
					mensajeAlarma ("Se ingreso demasiadas veces la clave incorrecta.\nAlarma activada");
					alarmaEstado = DISPARADA;

				}

			}
		}

		if(tout1.estado == FINALIZO) {										// termino el timeout por esperar pass
			tout1.estado = INACTIVO;										// bajo el flag del timeout
			cantidadIntentosClaveMal = 0;									// reinicio el contador de intentos
			alarmaEstado = DESARMADA;										// vuelvo al estado inicial
		}



		break;

	case USUARIO_SALIENDO:

		if(tout1.estado == FINALIZO) {										// termino el timeout por esperar salida de usuario
			tout1.estado = INACTIVO;										// bajo el flag del timeout
			cantidadIntentosClaveMal = 0;									// reinicio el contador de intentos
			alarmaEstado = ARMADA;											// voy al estado armada
		}

		if(sensorVentanaActivado())
			alarmaEstado = DISPARADA;

		break;
	case ARMADA:

		if(sensorVentanaActivado()) {
			alarmaEstado = DISPARADA;
		}
		else if(sensorPuertaActivado()) {
			alarmaEstado = USUARIO_ENTRANDO;
			activarTimeout(&tout1, ALARMA_TIMEOUT_ENTRANDO_MS);				// empiezo a contar timeout para ENTRAR
		}

		break;
	case DISPARADA:
		if(colaRx.datosNuevos){												// pregunto si tengo datos nuevos
			colaRx.datosNuevos = 0;											// bajo el flag de datos nuevos
			lineaColaAString(cadena, CADENA_L, colaRx);						// saco linea de la cola de datos a un string

			if(strcmp(cadena, ALARMA_STRING_PASSWORD)) {
				mensajeAlarma("Alarma desarmada");
				alarmaEstado = DESARMADA;
			}
			else {
				// Recibi un comando incorrecto
				mensajeAlarma ("Contraseña incorrecta");					// mando mensaje al display
			}
		}
		break;


	case USUARIO_ENTRANDO:

		if(colaRx.datosNuevos){												// pregunto si tengo datos nuevos
			colaRx.datosNuevos = 0;											// bajo el flag de datos nuevos
			lineaColaAString(cadena, CADENA_L, colaRx);						// saco linea de la cola de datos a un string

			if(strcmp(cadena, ALARMA_STRING_PASSWORD)) {
				mensajeAlarma("Alarma desarmada");
				alarmaEstado = DESARMADA;
			}
			else {
				// Recibi un comando incorrecto
				mensajeAlarma ("Contraseña incorrecta");					// mando mensaje al display
				cantidadIntentosClaveMal++;									// incremento el contador de clave erronea

				if(cantidadIntentosClaveMal >= ALARMA_CANTIDAD_INTENTOS_CLAVE_MAL_MAX) {
					mensajeAlarma ("Se ingreso demasiadas veces la clave incorrecta.\nAlarma activada");
					alarmaEstado = DISPARADA;
				}

			}
		}
		else if(tout1.estado == FINALIZO) {										// termino el timeout por esperar salida de usuario
			tout1.estado = INACTIVO;										// bajo el flag del timeout
			alarmaEstado = DISPARADA;										// voy al estado armada
			cantidadIntentosClaveMal = 0;
		}
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
 * @fn bool sensorVentanaActivado (void)
 *
 * @brief chequeo si alguno de los sensores de ventana se activo
 *
 */

bool_t sensorVentanaActivado (void) {

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
 * @fn bool sensorPuertaActivado (void)
 *
 * @brief chequeo si alguno de los sensores de ventana se activo
 *
 */

bool_t sensorPuertaActivado (void) {

	if(antirreboteMEF_puerta.t == TECLA_PRESIONADA){
		antirreboteMEF_ventana1.t = TECLA_SUELTA;
		return TRUE;
	}

}


/**
 * @fn mensajeAlarma (int8_t * mensaje)
 *
 * @brief envio un mensaje por el display de la alarma
 *
 */

void mensajeAlarma (int8_t * mensaje) {


	int8_t mensajeCompleto[CADENA_L];

	sprintf(mensajeCompleto, "%s\n\r", mensaje);

	if(EscribirCadenaCola (mensajeCompleto, &colaTx) != ESCRIBIR_COLA_COLA_LLENA)

		return;


}
