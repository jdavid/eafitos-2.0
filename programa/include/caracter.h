//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/caracter.h
//	Definici�n de la lista de controladores de tipo caracter.
// --------------------------------------------------------------------

#ifndef _DISPOSITIVOS_CARACTER
#define _DISPOSITIVOS_CARACTER

#include <ccar.h>


#define NUM_DISPOS_CARACTER 5


// Tabla de dispositivos de tipo caracter
class DispositivosCaracter {
	// Tabla de controladores de tipo car�cter
	ControladorCaracter* dispositivos[NUM_DISPOS_CARACTER];
public:
	void iniciar();
	void terminar() {}

	// Devuelve el controlador con el identificador especificado
	ControladorCaracter* obtenerControlador(char* nombre);

	// M�todo encargado de gestionar las colas de hilos asociadas a
	//   los controladores de tipo car�cter cada vez que se produce
	//   una interrupci�n de reloj
	long gestionarColas();
};


#endif
