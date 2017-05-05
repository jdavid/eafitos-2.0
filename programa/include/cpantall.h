//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/cpantall.h
//	Definición del controlador de la pantalla.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_PANTALLA
#define _CONTROLADOR_PANTALLA

#include <ccar.h>


class ControladorPantalla : public ControladorCaracter {
public:
	ControladorPantalla(char* n) : ControladorCaracter(n) {}

	void _escribirCaracter(char caracter);
};


#endif
