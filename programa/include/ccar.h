//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/ccar.h	
//	Definici�n del controlador de tipo caracter.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_DISPOSITIVO_CARACTER
#define _CONTROLADOR_DISPOSITIVO_CARACTER

#include <controla.h>


class ControladorCaracter : public Controlador {
	// M�todo encargado de resolver las peticiones de entrada/salida
	long ejecutarPeticion();
protected:
	// Identificador del controlador
	char* nombre;
public:
	ControladorCaracter(char* n);
	virtual ~ControladorCaracter();

	// M�todo que nos dice si este controlador es el buscado
	long es(char* n);

	// M�todos utilizados para realizar las peticiones de entrada/salida
	char leerCaracter();
	void escribirCaracter(char caracter);

	// M�todos que ejecutan las peticiones
	virtual char _leerCaracter()
		{ throw "funci�n no implementada"; }

	virtual void _escribirCaracter(char caracter)
		{ throw "funci�n no implementada"; }
};


#endif
