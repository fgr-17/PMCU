/**
 * @file uart.c
 *
 * @date Apr 12, 2018
 * @author froux
 */


/* =======================================================================================
 * 								INCLUSIÓN DE ARCHIVOS
 * =======================================================================================
 */

#include <stdint.h>
#include <string.h>
#include "sapi.h"

#include "colas_datos.h"
#include "uart.h"

/* =======================================================================================
 * 								VARIABLES GLOBALES
 * =======================================================================================
 */

t_cola colaTx;
t_cola colaRx;

/* =======================================================================================
 * 								IMPLEMENTACION DE FUNCIONES
 * =======================================================================================
 */

/**
 * @fn void InicializarUART (void)
 *
 * @brief Inicializacion de estructuras asociadas a la UART
 *
 */

void InicializarUART (void)
{

	InicializarCola(&colaRx);
	InicializarCola(&colaTx);
	return;
}


/**
 * @fn void taskUARTGetChar (void)
 *
 * @brief Descargo la fifo de la UART a la cola de recepcion.
 *
 * @author Roux, Federico G.
 */


void taskUARTGetChar (void){

	uint8_t lineaRecibida [UART_LINEA_RECIBIDA_N];

	memset(lineaRecibida, NULL, UART_LINEA_RECIBIDA_N);

	while(uartReadByte( UART_USB, lineaRecibida ) != FALSE) {
		EscribirCola(&colaRx, lineaRecibida[0]);
	}
	return;
}

/**
 * @fn void taskUARTPutChar (void)
 *
 * @brief Tarea que extrae los bytes de la cola de transmision y los escribe en la fifo de la UART
 *
 * @author Roux, Federico G.
 */

void taskUARTPutChar (void) {

	uint8_t byteLeido[2] = " ";

	while(LeerCola(&colaTx, byteLeido) != LEER_COLA_COLA_VACIA)
	{
		byteLeido[1] = '\0',
		printString(UART_USB, byteLeido);
	}

	return;
}

