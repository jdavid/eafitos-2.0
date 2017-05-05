//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/nucleo.h
//	Definici�n del n�cleo.
// --------------------------------------------------------------------

#ifndef _NUCLEO
#define _NUCLEO


#include <memoria.h>
#include <procesos.h>
#include <caracter.h>
#include <bloque.h>
#include <sf.h>


// Definici�n de llamadas al sistema
#define CREAR_HILO 1		// De gesti�n de hilos y procesos
#define TERMINAR_HILO 2
#define TERMINAR 3
#define CAMBIAR_UNIDAD 10	// Del sistema de ficheros
#define CAMBIAR_DIRECTORIO 11
#define CREAR_FICHERO 12
#define ABRIR_FICHERO 13
#define CERRAR_FICHERO 14
#define BORRAR_FICHERO 15
#define LEER_FICHERO 16
#define ESCRIBIR_FICHERO 17
#define SALTAR_FICHERO 18
#define EJECUTAR 20		// Para ejecutar un programa
#define LEER_ENTRADA_ESTANDAR 30	// De E/S estandar
#define ESCRIBIR_SALIDA_ESTANDAR 31


class Nucleo {
public:
	static ManejadorMemoria manejadorMemoria;
	static ManejadorProcesos manejadorProcesos;
	static DispositivosCaracter dispositivosCaracter;
	static DispositivosBloque dispositivosBloque;
	static SFV sfv;

	// Clases utilizadas como excepciones en el m�todo reloj, sirven para
	//   informar a la CPU.
	class NoHayHilos { };		// Ya no quedan hilos, la CPU debe
					//   dejar de ejecutar instrucciones
					//   y devolver el control.
	class NoHayHilosListos { };	// Quedan hilos pero todos est�n en
					//   SUSPENDIDO, la CPU no debe
					//   ejecutar ninguna instrucci�n
					//   este ciclo.

	static void iniciar();
	static void terminar();

	// M�todo equivalente a las rutinas de servicio de la interrupci�n
	//   de reloj que tienen los sistemas reales.
	static void reloj();
	// M�todo utilizado como interfaz para las llamadas al sistema.
	static void llamada();
};


#endif
