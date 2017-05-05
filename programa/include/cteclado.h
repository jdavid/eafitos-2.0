//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/cteclado.h
//	Definición del controlador del teclado.
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
