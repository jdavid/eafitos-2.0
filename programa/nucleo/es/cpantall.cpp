//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/cpantall.cpp
//	Implementaci�n del controlador de la pantalla.
// --------------------------------------------------------------------

#include <cpantall.h>
#include <mv.h>


void ControladorPantalla::_escribirCaracter(char caracter) {
	MaqVirtual::pantalla.escribirCaracter(caracter);
}
