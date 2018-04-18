/**
 *
 * @file main.c
 * @author Copyright 2017, Agustin Bassi. All rights reserved.
 *
 * @brief
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
 * @date 2017-11-24
 */

/*==================[inclusiones]============================================*/

#include "main.h"

#include "cooperativeOs_isr.h"       // <= dispatcher and task management header
#include "cooperativeOs_scheduler.h" // <= scheduler and system initialization header
#include "sapi.h"

#include "colas_datos.h"

#include "uart.h"
#include "leds.h"
#include "alarma.h"

/*==================[macros and definitions]=================================*/
DEBUG_PRINT_ENABLE

/*==================[definiciones de datos internos]=========================*/

static uint8_t menu[] =
		"\n\r"
		"********************* ALARMA *************************\n\r"
		"\n\r"
		"TEC1: Sensor Puerta.\n\r"
		"TEC2-3-4: Sensores ventanas 1-3.\n\r"
		"S: comando para salir del hogar. Espera 60s para salir.\n\r"
		;

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

typedef enum {BLINK_LED_ID,
			  ALARMA_MEF_ID, LEER_SENSORES_ID, TIMEOUT_ID,
			  UART_PUT_CHAR, UART_GET_CHAR} tareasID_t;

tareasID_t tareasID[SCHEDULER_MAX_TASKS];

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

int main( void ){

	//Inicializar el hardware, perifericos, puertos, clock, etc.
	boardConfig();
	// Inicializar UART_USB como salida de consola
	debugPrintConfigUart( UART_USB, UART_BAUD_RATE );
	debugPrintlnString("UART_USB configurada.\n\r" );

	inicializarTaskAlarmaMEF();
	InicializarUART();
	inicializarSensores();

	// uartWriteString(UART_USB, menu);

	// FUNCION que inicializa el planificador de tareas
	schedulerInit();

	// Cargar las tareas del sistema operativo con sus periodicidades
	tareasID[BLINK_LED_ID] = schedulerAddTask( (sAPI_FuncPtr_t) taskBlinkLed, 0, BLINK_LED_PERIODO );
	tareasID[ALARMA_MEF_ID] = schedulerAddTask( (sAPI_FuncPtr_t) taskAlarmaMEF, 1, ALARMA_MEF_PERIODO );
	tareasID[LEER_SENSORES_ID] = schedulerAddTask( (sAPI_FuncPtr_t) taskLeerSensores, 2, LEER_SENSORES_PERIODO );
	tareasID[TIMEOUT_ID] = schedulerAddTask( (sAPI_FuncPtr_t) taskTimeout, 3, TIMEOUT_PERIODO );
	// tareas de la UART
	tareasID[UART_PUT_CHAR] = schedulerAddTask( (sAPI_FuncPtr_t) taskUARTPutChar, 4, UART_PUT_CHAR_TIME_PERIOD );
	tareasID[UART_GET_CHAR] = schedulerAddTask( (sAPI_FuncPtr_t) taskUARTGetChar, 5, UART_GET_CHAR_TIME_PERIOD );

	//Iniciar el planificador de tareas
	schedulerStart( 1 );

	debugPrintlnString(menu);

	//printString(UART_USB, 27);

	while(TRUE) {
		//Ejecutar tareas
		schedulerDispatchTasks();
	}
	return 0;
}

/*==================[definiciones de funciones internas]=====================*/



/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/

