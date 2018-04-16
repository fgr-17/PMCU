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

#define BUF_STRING_L	32

uint8_t bufStringRX [BUF_STRING_L];


/* =======================================================================================
 * 								IMPLEMENTACION DE FUNCIONES
 * =======================================================================================
 */

/**
 * @brief void InicializarUART (void)
 *
 */

void InicializarUART (void)
{

	InicializarCola(&colaRx);
	InicializarCola(&colaTx);
	memset(bufStringRX, 0);
	return;
}


/**
 * @brief void taskUARTGetChar (void)
 *
 * @author Roux, Federico G.
 */

void taskUARTGetChar (void){

	uint8_t lineaRecibida [2] = " ";
	while(uartReadByte( UART_USB, lineaRecibida ) != FALSE) {
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

	while(LeerCola(&colaTx, byteLeido) != LEER_COLA_COLA_VACIA)
	{
		byteLeido[1] = '\0',
		printString(UART_USB, byteLeido);
	}

	return;
}

