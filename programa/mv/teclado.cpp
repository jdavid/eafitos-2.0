//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	mv/teclado.cpp
//	Implementaci�n del teclado virtual.
// --------------------------------------------------------------------

#include <curses.h>
#include <teclado.h>

char Teclado::leerCaracter() {
	return getch();
}
