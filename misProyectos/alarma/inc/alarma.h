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

#define STRING_L		32
#define CADENA_L		16

#define TIMEOUT_PERIODO		50

/* =================================== [Definicion de tipos de datos] ============================= */

/** @brief enumeracion con los estados de la maquina de estados de la alarma */
typedef enum {DESARMADA, ARMADA, DISPARADA, ESPERANDO_PASS, USUARIO_SALIENDO, USUARIO_ENTRANDO} alarma_t;

/** @brief dejo la variable de estado como externa para poder actualizar los leds en funcion de esto */
extern alarma_t alarmaEstado;

/** @brief estado del timeout. lo uso tambien como flag para mostrar que ya lo lei */
typedef enum {INACTIVO, CONTANDO, FINALIZO} toutEstado_t;

/** @brief estructura para manejo de timeouts */
typedef struct {
	bool_t 			activo;  // flag para indicar que empiece y termine de contar
	uint32_t	 	cuenta;  // valor instantaneo de la cuenta
	uint32_t		maximo;  // valor maximo para finalizar de contar
	toutEstado_t 	estado;  // estado del timeout
} timeout_t;

#endif /* MISPROYECTOS_ALARMA_INC_ALARMA_H_ */
