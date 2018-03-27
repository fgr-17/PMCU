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

/*==================[definiciones de datos internos]=========================*/

#define DELAY_DURATION 1000

#define ANTIRREBOTE_DELAY	40

fsmBlinky_t state = LED_ON;

fsmAntirrebote_t fsmAntirrebote_estado = BUTTON_UP;
fsmAntirrebote_t fsmAntirrebote2_estado = BUTTON_UP;

delay_t blinkyDelay;

delay_t antirreboteDelay;

gpioMap_t tecla_a1 = TEC1;
gpioMap_t tecla_a2 = TEC2;


tecla_t t1;
tecla_t t2;
/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
void fsmBlinky(void);
void fsmAntirrebote (void);
void Inicializar_fsmAntirrebote (void);
/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

#define LEDS_L ( sizeof(leds) / sizeof(gpioMap_t) )
#define PERIODOS_L ( sizeof(periodos) / sizeof( tick_t ) )

#define PERIODO_50			50
#define PERIODO_100			100

#define TECLA_ABAJO			OFF
#define TECLA_ARRIBA		ON

tick_t periodos[] = {PERIODO_50, PERIODO_100};
tick_t periodoActivo = PERIODO_50;
int periodoIndice = 0;

gpioMap_t leds[] = {LEDR, LEDG, LEDB};
gpioMap_t ledActivo = LEDR;
int ledIndice = 0;

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){




	// ---------- CONFIGURACIONES ------------------------------
	Inicializar_fsmAntirrebote();
	Inicializar_fsmAntirrebote2();
	// Inicializar y configurar la plataforma
	boardConfig();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		fsmBlinky();
		fsmAntirrebote();
		fsmAntirrebote2();

		if (t1 == TECLA_PRESIONADA){
			periodoIndice++;
			if (periodoIndice >= PERIODOS_L)
				periodoIndice = 0;

			periodoActivo = periodos[periodoIndice];


			t1 = TECLA_SUELTA;
		}

		if (t2 == TECLA_PRESIONADA){
			ledIndice++;
			if (ledIndice >= LEDS_L)
				ledIndice = 0;

			gpioWrite(ledActivo, 0);	// apago el led anterior
			ledActivo = leds[ledIndice];


			t2 = TECLA_SUELTA;
		}


		// Escribir el valor leido en el LED correspondiente.
		// gpioWrite( LEDR, tec1Value );




	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}



/*==================[definiciones de funciones internas]=====================*/


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
 * @fn void Inicializar_fsmAntirrebote (void)
 *
 *
 */


void Inicializar_fsmAntirrebote (void){

	fsmAntirrebote_estado = BUTTON_UP;
	t1 = TECLA_SUELTA;
}

/**
 *
 * @fn void fsmAntirrebote (void)
 *
 *
 *
 */

void fsmAntirrebote (void){

	bool_t tecValue = TECLA_ARRIBA;


	switch(fsmAntirrebote_estado)
	{

	case BUTTON_UP:
		tecValue = gpioRead( tecla_a1 );
		if(tecValue == TECLA_ARRIBA){
			fsmAntirrebote_estado = BUTTON_UP;
		}
		else if (tecValue == TECLA_ABAJO)
		{
			fsmAntirrebote_estado = BUTTON_FALLING;
			delayConfig(&antirreboteDelay, ANTIRREBOTE_DELAY);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreboteDelay);
		}

		break;
	case BUTTON_FALLING:

		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreboteDelay)){
			tecValue = gpioRead( tecla_a1 );
			if(tecValue == TECLA_ARRIBA){
				fsmAntirrebote_estado = BUTTON_UP;
			}
			else if(tecValue == TECLA_ABAJO){
				fsmAntirrebote_estado = BUTTON_DOWN;
				t1 = TECLA_PRESIONADA;
			}

		}
		else{
			fsmAntirrebote_estado = BUTTON_FALLING;
		}
		break;
	case  BUTTON_DOWN:

		tecValue = gpioRead( tecla_a1 );
		if(tecValue == TECLA_ABAJO){
			fsmAntirrebote_estado = BUTTON_DOWN;
		}
		else if (tecValue == TECLA_ARRIBA)
		{
			fsmAntirrebote_estado = BUTTON_RAISING;
			delayConfig(&antirreboteDelay, ANTIRREBOTE_DELAY);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreboteDelay);
		}
		break;

	case BUTTON_RAISING:


		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreboteDelay)){
			tecValue = gpioRead( tecla_a1 );
			if(tecValue == TECLA_ABAJO){
				fsmAntirrebote_estado = BUTTON_DOWN;
			}
			else if(tecValue == TECLA_ARRIBA){
				fsmAntirrebote_estado = BUTTON_UP;
			}

		}
		else{
			fsmAntirrebote_estado = BUTTON_RAISING;
		}
		break;

	default:
		Inicializar_fsmAntirrebote();

	}

	return;
}

void Inicializar_fsmAntirrebote2 (void){

	fsmAntirrebote_estado = BUTTON_UP;
	t2 = TECLA_SUELTA;
}

/**
 *
 * @fn void fsmAntirrebote (void)
 *
 *
 *
 */

void fsmAntirrebote2 (void){

	bool_t tecValue = TECLA_ARRIBA;


	switch(fsmAntirrebote2_estado)
	{

	case BUTTON_UP:
		tecValue = gpioRead( tecla_a2 );
		if(tecValue == TECLA_ARRIBA){
			fsmAntirrebote2_estado = BUTTON_UP;
		}
		else if (tecValue == TECLA_ABAJO)
		{
			fsmAntirrebote2_estado = BUTTON_FALLING;
			delayConfig(&antirreboteDelay, ANTIRREBOTE_DELAY);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreboteDelay);
		}

		break;
	case BUTTON_FALLING:

		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreboteDelay)){
			tecValue = gpioRead( tecla_a2 );
			if(tecValue == TECLA_ARRIBA){
				fsmAntirrebote2_estado = BUTTON_UP;
			}
			else if(tecValue == TECLA_ABAJO){
				fsmAntirrebote2_estado = BUTTON_DOWN;
				t2 = TECLA_PRESIONADA;
			}

		}
		else{
			fsmAntirrebote2_estado = BUTTON_FALLING;
		}
		break;
	case  BUTTON_DOWN:

		tecValue = gpioRead( tecla_a2 );
		if(tecValue == TECLA_ABAJO){
			fsmAntirrebote2_estado = BUTTON_DOWN;
		}
		else if (tecValue == TECLA_ARRIBA)
		{
			fsmAntirrebote2_estado = BUTTON_RAISING;
			delayConfig(&antirreboteDelay, ANTIRREBOTE_DELAY);
			/* para que el delay arranque hay que leerlo*/
			delayRead(&antirreboteDelay);
		}
		break;

	case BUTTON_RAISING:


		/* Si el tiempo del delay expiró, paso a leer el estado del pin */
		if (delayRead(&antirreboteDelay)){
			tecValue = gpioRead( tecla_a2 );
			if(tecValue == TECLA_ABAJO){
				fsmAntirrebote2_estado = BUTTON_DOWN;
			}
			else if(tecValue == TECLA_ARRIBA){
				fsmAntirrebote2_estado = BUTTON_UP;
			}

		}
		else{
			fsmAntirrebote2_estado = BUTTON_RAISING;
		}
		break;

	default:
		Inicializar_fsmAntirrebote2();

	}

	return;
}


/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
