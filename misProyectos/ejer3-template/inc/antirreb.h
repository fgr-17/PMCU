/*
 * antirreb.h
 *
 *  Created on: Apr 6, 2018
 *      Author: piro2
 */

#ifndef MISPROYECTOS_EJER3_TEMPLATE_INC_ANTIRREB_H_
#define MISPROYECTOS_EJER3_TEMPLATE_INC_ANTIRREB_H_

#define ANTIRREBOTE_DELAY	40

#define TECLA_ABAJO			OFF
#define TECLA_ARRIBA		ON

typedef enum {BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING} fsmAntirrebote_t;
typedef enum {TECLA_PRESIONADA, TECLA_SUELTA, TECLA_ERROR} tecla_t;


typedef struct
{
	fsmAntirrebote_t estado;
	delay_t delay;
	tick_t tiempo_ventana;
	gpioMap_t tecla_pin;
	tecla_t t;
}antirreb_t;



extern antirreb_t antirreb_tecla1;
extern antirreb_t antirreb_tecla2;



#endif /* MISPROYECTOS_EJER3_TEMPLATE_INC_ANTIRREB_H_ */
