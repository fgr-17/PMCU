/*
 * leds.h
 *
 *  Created on: Apr 6, 2018
 *      Author: piro2
 */

#ifndef MISPROYECTOS_EJER3_TEMPLATE_INC_LEDS_H_
#define MISPROYECTOS_EJER3_TEMPLATE_INC_LEDS_H_

/* =========================================================================================
 * 					DECLARACION DE VARIABLES
 * =========================================================================================
 */

#define LEDS_L ( sizeof(leds) / sizeof(gpioMap_t) )
#define PERIODOS_L ( sizeof(periodos) / sizeof( tick_t ) )


/* =========================================================================================
 * 					FCS EXTERNAS
 * =========================================================================================
 */

extern void taskActualizarLeds();
extern void taskBlinkLed(void);

extern int32_t tareaBlinkyID;


#endif /* MISPROYECTOS_EJER3_TEMPLATE_INC_LEDS_H_ */
