//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	mv/pantalla.cpp
//	Implementaci�n de la pantalla virtual.
// --------------------------------------------------------------------

#include <curses.h>
#include <pantalla.h>

void Pantalla::escribirCaracter(char caracter) {
	addch(caracter);
	refresh();
}
