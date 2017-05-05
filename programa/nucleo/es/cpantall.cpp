//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/cpantall.cpp
//	Implementación del controlador de la pantalla.
// --------------------------------------------------------------------

#include <cpantall.h>
#include <mv.h>


void ControladorPantalla::_escribirCaracter(char caracter) {
	MaqVirtual::pantalla.escribirCaracter(caracter);
}
