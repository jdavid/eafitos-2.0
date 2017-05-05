//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/cteclado.cpp
//	Implementación del controlador del teclado.
// --------------------------------------------------------------------

#include <cteclado.h>
#include <mv.h>

char ControladorTeclado::_leerCaracter() {
	return MaqVirtual::teclado.leerCaracter();
}
