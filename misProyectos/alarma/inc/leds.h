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

#define DELAY_L				5
#define BLINK_LED_PERIODO	50

#define ENCENDER_LED(x) 	gpioWrite(x, ON)
#define APAGAR_LED(x) 		gpioWrite(x, OFF)

/* =========================================================================================
 * 					FCS EXTERNAS
 * =========================================================================================
 */

extern void taskActualizarLeds();
extern void taskBlinkLed(void);

#endif /* MISPROYECTOS_EJER3_TEMPLATE_INC_LEDS_H_ */
