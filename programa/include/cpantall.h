//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/cpantall.h
//	Definici�n del controlador de la pantalla.
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
