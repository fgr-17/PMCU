/* Copyright 2017, Agustin Bassi.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Date: 2017-11-24
 */

/*==================[inlcusiones]============================================*/

#include "main.h"

#include "cooperativeOs_isr.h"       // <= dispatcher and task management header
#include "cooperativeOs_scheduler.h" // <= scheduler and system initialization header
#include "sapi.h"

#include "colas_datos.h"

#include "alarma.h"

#include "uart.h"
#include "leds.h"

/*==================[macros and definitions]=================================*/
DEBUG_PRINT_ENABLE

#define BLINK_TIME_PERIOD 50
#define CONSOLE_TIME_PERIOD 50

/*==================[definiciones de datos internos]=========================*/

static uint8_t menu[] =
		"\n\r"
		"********************* MENU *************************\n\r"
		"\n\r"
		"TEC1: cambia el tiempo de parpadeo del led activo.\n\r"
		"TEC2: cambia el led activo.\n\r"
		"T: permite ingresar un tiempo desde el puerto serie.\n\r"
		"L: permite ingresar un led desde el puerto serie.\n\r"
		;

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

int main( void ){

	//Inicializar el hardware, perifericos, puertos, clock, etc.
	boardConfig();
	// Inicializar UART_USB como salida de consola
	debugPrintConfigUart( UART_USB, UART_BAUD_RATE );
	debugPrintlnString( "UART_USB configurada.\n\r" );

	InicializarCola(&colaRx);
	InicializarCola(&colaTx);
	inicializarTaskAlarmaMEF();
	inicializarSensores();
	inicializarTimeout();

	// uartWriteString(UART_USB, menu);

	//FUNCION que inicializa el planificador de tareas
	schedulerInit();
	//Cargar las tareas del sistema operativo con sus periodicidades
	schedulerAddTask( (sAPI_FuncPtr_t) taskBlinkLed, 0, BLINK_TIME_PERIOD );
 	schedulerAddTask( (sAPI_FuncPtr_t) taskLeerSensores, 1, LEER_SENSORES_PERIODO );
 	schedulerAddTask( (sAPI_FuncPtr_t) taskTimeout, 2, TIMEOUT_PERIODO );
	schedulerAddTask( (sAPI_FuncPtr_t) taskAlarmaMEF, 3, ALARMA_MEF_PERIODO );

	schedulerAddTask( (sAPI_FuncPtr_t) taskUARTPutChar, 4, UART_PUT_CHAR_TIME_PERIOD );
	schedulerAddTask( (sAPI_FuncPtr_t) taskUARTGetChar, 5, UART_GET_CHAR_TIME_PERIOD );


	//Iniciar el planificador de tareas
	schedulerStart( 1 );

	debugPrintlnString(menu);
	//Lazo infinito

	while(TRUE) {
		//Ejecutar tareas
		schedulerDispatchTasks();
	}
	return 0;
}

/*==================[definiciones de funciones internas]=====================*/



/**
 * @fn static void taskMenuUpdate(void)
 *
 * @brief Tarea de impresion de menu
 *
 */

static void taskMenuUpdate(void){

	uint8_t receiveByte;

	if (uartReadByte( UART_USB, &receiveByte ) != FALSE) {

		if(receiveByte != 0)
			switch (receiveByte) {
			case 't':
			case 'T':
				uartWriteString(UART_USB, menu);
				debugPrintlnString( "Caracter recibido: T\n\r");
				break;

			case 'L':
				uartWriteString(UART_USB, menu);
				uartWriteString(UART_USB, "Caracter recibido: L\n\r");
				break;

			case '\n':
			case '\r':
				break;

			default:
				uartWriteString(UART_USB, menu);
				uartWriteString(UART_USB, "Caracter recibido fuera de menu\n\r");
				break;
			}
	}
}
/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

