/**
 *
 * @file	colas.c
 *
 * @brief 	Funciones relacionadas con el manejo de colas
 *
 * @date 	Apr 12, 2018
 *
 * @author 	Roux, Federico G. (froux@favaloro.edu.ar)
 * @author	Laboratorio de uP - UNIVERSIDAD FAVALORO 2014
 *
 */

/* =======================================================================================
 * 								INCLUSIÓN DE ARCHIVOS
 * =======================================================================================
 */

#include <stdint.h>
#include "colas_datos.h"

/* =======================================================================================
 * 							PROTOTIPOS DE FUNCIONES
 * =======================================================================================
 */

void InicializarCola (t_cola* cola);
int8_t EscribirCola (t_cola*cola, uint8_t dato_a_escribir);
int8_t LeerCola(t_cola*cola, uint8_t*dato_leido);
int32_t EscribirCadenaCola (uint8_t *cadena, t_cola*cola);

/* =======================================================================================
 * 								VARIABLES GLOBALES
 * =======================================================================================
 */

/* =======================================================================================
 * 							IMPLEMENTACION DE FUNCIONES
 * =======================================================================================
 */

/**
 * @brief	Inicializo la estructura del tipo cola
 *
 * @params 	cola 	estructura a inicializar
 *
 * @returns
 */

void InicializarCola (t_cola* cola)
{

	cola->ini = BUFFER_N - 1;							// Inicializo el "ini" en el último dato
	cola->fin = 0;										// Inicializo "fin" al comienzo del buffer
	// cola->datos_pendientes = 0;							// Inicializo el contador de datos pendientes en cero
	cola->estadoBuffer = BUFFER_VACIO;					// Señalizo la estructura como buffer vacío

	return;
}


/**
 *	@brief 		Escribo un dato en el buffer
 *
 * 	@params 	canal : Canal a escribir
 * 	@params		dato_a_escribir :
 *
 * 	@returns 	ESCRIBIR_DATO_COLA_EXITO	Escribió el dato en la cola
 * 	@returns 	ESCRIBIR_DATO_COLA_COLA_LLENA
 *
 */

int8_t EscribirCola (t_cola*cola, uint8_t dato_a_escribir)
{
	if(cola->estadoBuffer == BUFFER_LLENO)						// Pregunto si el buffer ya está lleno
		return ESCRIBIR_COLA_COLA_LLENA;						// Retorno error de buffer lleno

	cola->buffer[cola->fin] = dato_a_escribir;					// Escribo el dato en el frente
	cola->fin++;												// Incremento el puntero del frente
	// cola->datos_pendientes++;									// Incremento el contador de datos pendientes
	cola->estadoBuffer = BUFFER_CARGANDO;						// Señalizo buffer cargando

	if(cola->fin >= BUFFER_N)									// Pregunto si el puntero del frente llegó al final
		cola->fin = 0;											// Lo inicializo al comienzo

	if(((cola->ini + 1) % BUFFER_N) == cola->fin)				// Condición para buffer vacío
		cola->estadoBuffer = BUFFER_LLENO;						// es porque el buffer está lleno

	return ESCRIBIR_COLA_EXITO;
}

/**
 *	@brief 		Leo y extraigo un dato de la cola
 *
 * 	@params 	cola : Cola a leer
 * 	@params		dato_leido : puntero a la variable donde escribo
 *
 * 	@returns 	none
 *
 */

int8_t LeerCola(t_cola*cola, uint8_t*dato_leido)
{

	if(cola->estadoBuffer == BUFFER_VACIO)						// Si el buffer está vacío
		return LEER_COLA_COLA_VACIA;							// Señalizo y salgo

	cola->ini++;												// Incremento el índice del fondo
	if(cola->ini >= BUFFER_N)									// Pregunto si llegué al final
		cola->ini = 0;											// Lo vuelvo a poner al principio

	*dato_leido = cola->buffer[cola->ini];						// Guardo el dato leido

	if(((cola->ini + 1) % BUFFER_N) == cola->fin)				// Condición para buffer vacío
		cola->estadoBuffer = BUFFER_VACIO;						// Caso verdadero: señalizo

	return LEER_COLA_EXITO;										// Retorno éxito en la lectura
}

/**
 *	@brief 		Escribo una cadena en una cola de datos
 *
 * 	@params 	cola : Cola a leer
 * 	@params		dato_leido : puntero a la variable donde escribo
 *
 * 	@returns 	none
 *
 */

int32_t EscribirCadenaCola (uint8_t *cadena, t_cola*cola)
{

	uint32_t i = 0;

	while(cadena[i] != '\0') {

		/* Intento agregar el dato a la cola de datos */
		if(EscribirCola(cola, cadena[i]) == ESCRIBIR_COLA_COLA_LLENA)
			/* si esta llena, retorno con error */
			return -1;
		i++;

	}

	return 0;
}

