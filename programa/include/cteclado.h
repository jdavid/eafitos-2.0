//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/cteclado.h
//	Definici�n del controlador del teclado.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_TECLADO
#define _CONTROLADOR_TECLADO

#include <ccar.h>


class ControladorTeclado : public ControladorCaracter {
public:
	ControladorTeclado(char* n) : ControladorCaracter(n) {}

	char _leerCaracter();
};


#endif
