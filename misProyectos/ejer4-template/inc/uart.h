/*
 * uart.h
 *
 *  Created on: Apr 12, 2018
 *      Author: piro2
 */

#ifndef MISPROYECTOS_EJER4_TEMPLATE_INC_UART_H_
#define MISPROYECTOS_EJER4_TEMPLATE_INC_UART_H_

/* ======================================================================================
 * 								DECLARACION DE CONSTANTES
 * ======================================================================================
 */

#define UART_GET_CHAR_TIME_PERIOD 50
#define UART_PUT_CHAR_TIME_PERIOD 50

#define UART_BAUD_RATE 115200
/* ======================================================================================
 * 								FUNCIONES EXTERNAS
 * ======================================================================================
 */

extern void taskUARTGetChar (void);
extern void taskUARTPutChar (void);

/* =======================================================================================
 * 								VARIABLES GLOBALES
 * =======================================================================================
 */

extern t_cola colaTx;
extern t_cola colaRx;


#endif /* MISPROYECTOS_EJER4_TEMPLATE_INC_UART_H_ */
