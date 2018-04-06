/*
 * leds.h
 *
 *  Created on: Apr 6, 2018
 *      Author: piro2
 */

#ifndef MISPROYECTOS_EJER3_TEMPLATE_INC_LEDS_H_
#define MISPROYECTOS_EJER3_TEMPLATE_INC_LEDS_H_

#define LEDS_L ( sizeof(leds) / sizeof(gpioMap_t) )
#define PERIODOS_L ( sizeof(periodos) / sizeof( tick_t ) )



extern void ActualizarFrecuenciaParpadeo();
extern void ActualizarLedActivo();


extern gpioMap_t ledActivo;
extern tick_t periodoActivo;



#endif /* MISPROYECTOS_EJER3_TEMPLATE_INC_LEDS_H_ */
