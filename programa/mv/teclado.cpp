//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	mv/teclado.cpp
//	Implementación del teclado virtual.
// --------------------------------------------------------------------

#include <curses.h>
#include <teclado.h>

char Teclado::leerCaracter() {
	return getch();
}
