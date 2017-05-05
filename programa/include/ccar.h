//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/ccar.h	
//	Definición del controlador de tipo caracter.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_DISPOSITIVO_CARACTER
#define _CONTROLADOR_DISPOSITIVO_CARACTER

#include <controla.h>


class ControladorCaracter : public Controlador {
	// Método encargado de resolver las peticiones de entrada/salida
	long ejecutarPeticion();
protected:
	// Identificador del controlador
	char* nombre;
public:
	ControladorCaracter(char* n);
	virtual ~ControladorCaracter();

	// Método que nos dice si este controlador es el buscado
	long es(char* n);

	// Métodos utilizados para realizar las peticiones de entrada/salida
	char leerCaracter();
	void escribirCaracter(char caracter);

	// Métodos que ejecutan las peticiones
	virtual char _leerCaracter()
		{ throw "función no implementada"; }

	virtual void _escribirCaracter(char caracter)
		{ throw "función no implementada"; }
};


#endif
