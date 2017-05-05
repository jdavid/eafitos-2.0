//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/caracter.h
//	Definición de la lista de controladores de tipo caracter.
// --------------------------------------------------------------------

#ifndef _DISPOSITIVOS_CARACTER
#define _DISPOSITIVOS_CARACTER

#include <ccar.h>


#define NUM_DISPOS_CARACTER 5


// Tabla de dispositivos de tipo caracter
class DispositivosCaracter {
	// Tabla de controladores de tipo carácter
	ControladorCaracter* dispositivos[NUM_DISPOS_CARACTER];
public:
	void iniciar();
	void terminar() {}

	// Devuelve el controlador con el identificador especificado
	ControladorCaracter* obtenerControlador(char* nombre);

	// Método encargado de gestionar las colas de hilos asociadas a
	//   los controladores de tipo carácter cada vez que se produce
	//   una interrupción de reloj
	long gestionarColas();
};


#endif
