/*
 * antirreb.h
 *
 *  Created on: Apr 6, 2018
 *      Author: piro2
 */

#ifndef MISPROYECTOS_ALARMA_INC_ANTIRREB_H_
#define MISPROYECTOS_ALARMA_INC_ANTIRREB_H_

/* ========================== [ declaracion de constantes ] ======================= */

/** @brief delay de la ventana de antirrebote */
#define ANTIRREBOTE_DELAY	40
/** @brief valor logico de tecla presionada */
#define TECLA_ABAJO			OFF
/** @brief valor logico de tecla suelta */
#define TECLA_ARRIBA		ON

/* ========================== [ definicion de tipos de datos ] ======================= */

/** @brief enumeracion de estados de la maquina de estados de antirrebote */
typedef enum {BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING} antirreboteMEF_t;
/** @brief enumeracion de valores del flag de tecla */
typedef enum {TECLA_PRESIONADA, TECLA_SUELTA, TECLA_ERROR} tecla_t;

typedef struct
{
	antirreboteMEF_t estado;
	delay_t delay;
	tick_t tiempo_ventana;
	gpioMap_t tecla_pin;
	tecla_t t;
}antirreb_t;

/* ========================== [ variables externas ] ======================= */

extern antirreb_t antirreb_tecla1;
extern antirreb_t antirreb_tecla2;
extern antirreb_t antirreb_tecla3;
extern antirreb_t antirreb_tecla4;


#endif /* MISPROYECTOS_EJER3_TEMPLATE_INC_ANTIRREB_H_ */
