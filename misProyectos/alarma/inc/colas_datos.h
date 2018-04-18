/*
 * cola_datos.h
 *
 *  Created on: Apr 12, 2018
 *      Author: piro2
 */

#ifndef MISPROYECTOS_EJER4_TEMPLATE_INC_COLAS_DATOS_H_
#define MISPROYECTOS_EJER4_TEMPLATE_INC_COLAS_DATOS_H_


/* ======================================================================================
 * 								DECLARACION DE CONSTANTES
 * ======================================================================================
 */

/** @brief tamano del buffer de la cola de datos */
#define BUFFER_N 					256

#define ESCRIBIR_COLA_COLA_LLENA	-1
#define ESCRIBIR_COLA_EXITO			 0

#define LEER_COLA_COLA_VACIA		-1
#define LEER_COLA_EXITO				 0

/* ======================================================================================
 * 								DECLARACION DE TIPOS
 * ======================================================================================
 */

typedef enum
{
	BUFFER_LLENO,																		// Se completaron todos los bytes de la cola
	BUFFER_CARGANDO,																	// El buffer no esta vacío pero se está cargando
	BUFFER_VACIO,																		// Buffer completamente vacío
	BUFFER_TIMEOUT																		// Timeout de recepcion del frame
} t_estadoBuffer;


typedef struct
{
	uint8_t buffer[BUFFER_N];															// Buffer de la cola
	uint8_t ini, fin;																	// Índices de inicio y fin
	t_estadoBuffer estadoBuffer;														// estado del buffer
	uint8_t datosNuevos;																// Flag para indicar si hay datos que todavía no leí
} t_cola;

/* ======================================================================================
 * 								FUNCIONES EXTERNAS
 * ======================================================================================
 */

extern void InicializarCola (t_cola* cola);
extern int8_t EscribirCola (t_cola*cola, uint8_t dato_a_escribir);
extern int8_t LeerCola(t_cola*cola, uint8_t*dato_leido);
extern int32_t EscribirCadenaCola (uint8_t *cadena, t_cola*cola);
extern int32_t lineaColaAString (int8_t*string, int32_t largo, t_cola*cola);
/* ======================================================================================
 * 								VARIABLES EXTERNAS
 * ======================================================================================
 */


extern t_cola colaTx;
extern t_cola colaRx;

#endif /* MISPROYECTOS_EJER4_TEMPLATE_INC_COLAS_DATOS_H_ */
