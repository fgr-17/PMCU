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
 * @brief void taskUARTGetChar (void)
 *
 * @author Roux, Federico G.
 */

void taskUARTGetChar (void){

	uint8_t lineaRecibida [2] = " ";
	if (uartReadByte( UART_USB, lineaRecibida ) != FALSE) {
		lineaRecibida[1] = '\0',
		EscribirCola(&colaRx, lineaRecibida[0]);
	}
	return;
}

/**
 * @brief void taskUARTPutChar (void)
 *
 * @author Roux, Federico G.
 */

void taskUARTPutChar (void) {

	uint8_t byteLeido[2] = " ";

	if(LeerCola(&colaTx, byteLeido) != LEER_COLA_COLA_VACIA)
	{
		byteLeido[1] = '\0',
		printString(UART_USB, byteLeido);
	}

	return;
}

