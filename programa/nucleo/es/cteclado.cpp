//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/cteclado.cpp
//	Implementaci�n del controlador del teclado.
// --------------------------------------------------------------------

#include <cteclado.h>
#include <mv.h>

char ControladorTeclado::_leerCaracter() {
	return MaqVirtual::teclado.leerCaracter();
}
