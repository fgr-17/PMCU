/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "sapi.h"        // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

DEBUG_PRINT_ENABLE
CONSOLE_PRINT_ENABLE

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/


#define PERIODO_A 		50
#define PERIODO_B		100
#define PERIODO_INICIAL PERIODO_A

#define LED_INICIAL 	LEDR
// #define LED_CANTIDAD	6
#define LED_CANTIDAD	3


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

	tick_t periodo = PERIODO_INICIAL;
	// gpioMap_t leds[LED_CANTIDAD] = {LEDR, LEDG, LEDB, LED1, LED2, LED3};
	gpioMap_t leds[LED_CANTIDAD] = {LEDR, LEDG, LEDB};
	gpioMap_t ledActivo = leds[0];
	int indiceLed = 0, i;


   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializar UART_USB como salida Serial de debug
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "DEBUG: UART_USB configurada." );
   
   // Inicializar UART_232 como salida Serial de consola
   consolePrintConfigUart( UART_232, 115200 );
   consolePrintlnString( "UART_232 configurada." );

   // Crear varias variables del tipo booleano
   bool_t tec1Value = OFF;
   bool_t tec2Value = OFF;
   bool_t tec3Value = OFF;
   bool_t tec4Value = OFF;
   bool_t ledbValue = OFF;

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      /* Si se presiona TEC1, enciende el LEDR */

      // Leer pin conectado a la tecla.
      tec1Value = gpioRead( TEC1 );
      // Invertir el valor leido, pues lee un 0 (OFF) con tecla
      // presionada y 1 (ON) al liberarla.
      tec1Value = !tec1Value;
      // Escribir el valor leido en el LED correspondiente.
      // gpioWrite( LEDR, tec1Value );
      if(tec1Value == TRUE) {
    	  if (periodo == PERIODO_A)
    		  periodo = PERIODO_B;
    	  else
    		  periodo = PERIODO_A;
      }


      /* Si se presiona TEC2, cambia el color de lled */

      // Leer pin conectado a la tecla.
      tec2Value = gpioRead( TEC2 );
      // Invertir el valor leido, pues lee un 0 (OFF) con tecla
      // presionada y 1 (ON) al liberarla.
      tec2Value = !tec2Value;
      // Escribir el valor leido en el LED correspondiente.
      // gpioWrite( LED1, tec2Value );
      if(tec2Value == TRUE){
    	  ledActivo = leds[indiceLed];
    	  indiceLed++;
    	  if(indiceLed >= LED_CANTIDAD)
    		  indiceLed = 0;

          for(i = 0; i < LED_CANTIDAD; i++)
			  gpioWrite(leds[i], 0);

      }

      /* Intercambiar el valor del pin conectado a ledActivo */
      gpioToggle( ledActivo );

      /* Si se presiona TEC3, enciende el LED2 */

//      // Leer pin conectado a la tecla.
//      tec3Value = gpioRead( TEC3 );
//      // Invertir el valor leido, pues lee un 0 (OFF) con tecla
//      // presionada y 1 (ON) al liberarla.
//      tec3Value = !tec3Value;
//      // Escribir el valor leido en el LED correspondiente.
//      gpioWrite( LED2, tec3Value );
//
//
//      /* Si se presiona TEC4, enciende el LED3 */
//
//      // Leer pin conectado a la tecla.
//      tec4Value = gpioRead( TEC4 );
//      // Invertir el valor leido, pues lee un 0 (OFF) con tecla
//      // presionada y 1 (ON) al liberarla.
//      tec4Value = !tec4Value;
//      // Escribir el valor leido en el LED correspondiente.
//      gpioWrite( LED3, tec4Value );
//
//
//
//
//
//      /* Mostrar por UART_USB (8N1 115200) el estado del LEDB */
//
//      // Leer el estado del pin conectado al led
//      ledbValue = gpioRead( LEDB );
//      // Chequear si el valor leido es encedido
//      if( ledbValue == ON ){
//         // Si esta encendido mostrar por UART_USB "LEDB encendido."
//         debugPrintlnString( "DEBUG: LEDB encendido." );
//         consolePrintlnString( "LEDB encendido." );
//         consolePrintEnter();
//      } else{
//         // Si esta apagado mostrar por UART_USB "LEDB apagado."
//         debugPrintlnString( "DEBUG: LEDB apagado." );
//         consolePrintlnString( "LEDB apagado." );
//         consolePrintEnter();
//      }


      /* Retardo bloqueante durante 250ms */

      delay( periodo );
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
