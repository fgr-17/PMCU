/**
 * @file alarma.h
 *
 * @date 	Created on: 16/4/2018
 * @author piro2
 */

#ifndef MISPROYECTOS_ALARMA_INC_ALARMA_H_
#define MISPROYECTOS_ALARMA_INC_ALARMA_H_

/* =================================== [Declaracion de constantes] ============================= */

/** @brief periodo de actualizacion de la mef */
#define ALARMA_MEF_TIME_PERIOD		50
/** @brief Estado inicial de la maquina de estados */
#define ALARMA_MEF_ESTADO_INICIAL  	DESARMADA

#define ALARMA_SENSOR_PUERTA 				TEC1
#define ALARMA_SENSOR_VENTANA1				TEC2
#define ALARMA_SENSOR_VENTANA2				TEC3
#define ALARMA_SENSOR_VENTANA3				TEC4

#define ALARMA_LED_DESARMADA				LEDG
#define ALARMA_LED_ARMADA					LEDR
#define ALARMA_LED_DISPARADA				LEDR
#define ALARMA_LED_ESPERANDO_PASS			LEDB





/* =================================== [Definicion de tipos de datos] ============================= */

/** @brief enumeracion con los estados de la maquina de estados de la alarma */
typedef enum {DESARMADA, ARMADA, DISPARADA, ESPERANDO_PASS, USUARIO_SALIENDO, USUARIO_ENTRANDO} alarma_t;

/** @brief dejo la variable de estado como externa para poder actualizar los leds en funcion de esto */
extern alarma_t alarmaEstado;


#endif /* MISPROYECTOS_ALARMA_INC_ALARMA_H_ */
