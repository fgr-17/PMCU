/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

/*==================[inclusiones]============================================*/

#include "sapi.h"        // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

typedef enum {LED_ON, LED_OFF} fsmBlinky_t;

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


/*==================[definiciones de datos internos]=========================*/

#define DELAY_DURATION 1000

#define ANTIRREBOTE_DELAY	40

fsmBlinky_t state = LED_ON;
delay_t blinkyDelay;

/**
 * @
 *
 */


antirreb_t antirreb_tecla1;
antirreb_t antirreb_tecla2;

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
void fsmBlinky(void);

void fsmAntirrebote1 (void);
void Inicializar_fsmAntirrebote1 (void);

void fsmAntirrebote2 (void);
void Inicializar_fsmAntirrebote2 (void);

void fsmAntirrebote (antirreb_t*antirreb);
void Inicializar_fsmAntirrebote (antirreb_t*antirreb, gpioMap_t tecla_asigada);

void ActualizarFrecuenciaParpadeo();
void ActualizarLedActivo();

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

#define LEDS_L ( sizeof(leds) / sizeof(gpioMap_t) )
#define PERIODOS_L ( sizeof(periodos) / sizeof( tick_t ) )

#define TECLA_ABAJO			OFF
#define TECLA_ARRIBA		ON

tick_t periodos[] = {50, 100, 1000};
tick_t periodoActivo = 50;
int periodoIndice = 0;

gpioMap_t leds[] = {LEDR, LEDG, LEDB, LED1, LED2, LED3};
gpioMap_t ledActivo = LEDR;
int ledIndice = 0;

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

	// ---------- CONFIGURACIONES ------------------------------
	Inicializar_fsmAntirrebote(&antirreb_tecla1, TEC1);
	Inicializar_fsmAntirrebote(&antirreb_tecla2, TEC2);

	// Inicializar y configurar la plataforma
	boardConfig();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		fsmBlinky();
		fsmAntirrebote(&antirreb_tecla1);
		fsmAntirrebote(&antirreb_tecla2);
		ActualizarFrecuenciaParpadeo();
		ActualizarLedActivo();
	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}



/*==================[definiciones de funciones internas]=====================*/

/**
 * @fn void ActualizarFrecuenciaParpadeo()
 *
 *
 */

void ActualizarFrecuenciaParpadeo()
{
	if (antirreb_tecla1.t == TECLA_PRESIONADA){
		periodoIndice++;
		if (periodoIndice >= PERIODOS_L)
			periodoIndice = 0;

		periodoActivo = periodos[periodoIndice];


		antirreb_tecla1.t = TECLA_SUELTA;
	}
	return;
}

/**
 * @fn void ActualizarFrecuenciaParpadeo()
 *
 *
 */

void ActualizarLedActivo()
{
	if (antirreb_tecla2.t == TECLA_PRESIONADA){
		ledIndice++;
		if (ledIndice >= LEDS_L)
			ledIndice = 0;

		gpioWrite(ledActivo, 0);	// apago el led anterior
		ledActivo = leds[ledIndice];


		antirreb_tecla2.t = TECLA_SUELTA;
	}
	return;
}

/**
 * @fn fsmBlinky
 *
 *
 */

void fsmBlinky(void){

	switch(state){

	case LED_ON:
		/* Si el delay no está corriendo, lo configuro y cambio el estado del led*/
		if (!blinkyDelay.running){
			delayConfig(&blinkyDelay, periodoActivo);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&blinkyDelay);
			gpioToggle( ledActivo );
		}
		/* Si el tiempo del delay expiró, cambio el estado de la FSM*/
		if (delayRead(&blinkyDelay))
			state = LED_OFF;
		break;

	case LED_OFF:
		/* Si el delay no está corriendo, lo configuro y cambio el estado del led*/
		if (!blinkyDelay.running){
			delayConfig(&blinkyDelay, periodoActivo);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&blinkyDelay);
			gpioToggle( ledActivo );
		}
		/* Si el tiempo del delay expiró, cambio el estado de la FSM*/
		if (delayRead(&blinkyDelay))
			state = LED_ON;
		break;

	/* Condición de error, reinicio la máquina de estados*/
	default:
		state = LED_ON;
		break;


	}

	return;
}

/**
 * @fn void Inicializar_fsmAntirrebote2 (void)
 *
 */

void Inicializar_fsmAntirrebote (antirreb_t*antirreb, gpioMap_t tecla_asigada ){

	antirreb->estado = BUTTON_UP;
	antirreb->t = TECLA_SUELTA;
	antirreb->tiempo_ventana = ANTIRREBOTE_DELAY;
	antirreb->tecla_pin = tecla_asigada;
}

/**
 *
 * @fn void fsmAntirrebote (void)
 *
 *
 *
 */

void fsmAntirrebote (antirreb_t*antirreb){

	bool_t tecValue = TECLA_ARRIBA;


	switch(antirreb->estado)
	{

	case BUTTON_UP:
		tecValue = gpioRead( antirreb->tecla_pin );
		if(tecValue == TECLA_ARRIBA){
			antirreb->estado = BUTTON_UP;
		}
		else if (tecValue == TECLA_ABAJO)
		{
			antirreb->estado = BUTTON_FALLING;
			delayConfig(&antirreb->delay, antirreb->tiempo_ventana);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreb->delay);
		}

		break;
	case BUTTON_FALLING:

		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreb->delay)){
			tecValue = gpioRead( antirreb->tecla_pin );
			if(tecValue == TECLA_ARRIBA){
				antirreb->estado = BUTTON_UP;
			}
			else if(tecValue == TECLA_ABAJO){
				antirreb->estado = BUTTON_DOWN;
				antirreb->t = TECLA_PRESIONADA;
			}

		}
		else{
			antirreb->estado = BUTTON_FALLING;
		}
		break;
	case  BUTTON_DOWN:

		tecValue = gpioRead( antirreb->tecla_pin );
		if(tecValue == TECLA_ABAJO){
			antirreb->estado = BUTTON_DOWN;
		}
		else if (tecValue == TECLA_ARRIBA)
		{
			antirreb->estado = BUTTON_RAISING;
			delayConfig(&antirreb->delay, antirreb->tiempo_ventana);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreb->delay);
		}
		break;

	case BUTTON_RAISING:


		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreb->delay)){
			tecValue = gpioRead( antirreb->tecla_pin );
			if(tecValue == TECLA_ABAJO){
				antirreb->estado = BUTTON_DOWN;
			}
			else if(tecValue == TECLA_ARRIBA){
				antirreb->estado = BUTTON_UP;
			}

		}
		else{
			antirreb->estado = BUTTON_RAISING;
		}
		break;

	default:
		Inicializar_fsmAntirrebote(antirreb, antirreb->tecla_pin);

	}

	return;
}

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
