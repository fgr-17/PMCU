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

/**
 * @defgroup UART_DEFINES
 * @brief Constantes asociadas a la UART
 * @{
 */

/** @brief tiempo de actualizacion de la tarea GetChar */
#define UART_GET_CHAR_TIME_PERIOD 25
/** @brief tiempo de actualizacion de la tarea PutChar */
#define UART_PUT_CHAR_TIME_PERIOD 25
/** @brief Baud Rate de la UART */
#define UART_BAUD_RATE 115200
/** @brief cantidad de bytes del buffer temporal de recepcion */
#define UART_LINEA_RECIBIDA_N		32

/**
 * @}
 */

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




#endif /* MISPROYECTOS_EJER4_TEMPLATE_INC_UART_H_ */
