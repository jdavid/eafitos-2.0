//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	mv/pantalla.cpp
//	Implementación de la pantalla virtual.
// --------------------------------------------------------------------

#include <curses.h>
#include <pantalla.h>

void Pantalla::escribirCaracter(char caracter) {
	addch(caracter);
	refresh();
}
