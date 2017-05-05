//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/cdisco.cpp
//	Implementaci�n del controlador del disco virtual.
// --------------------------------------------------------------------

#include <cdisco.h>


ControladorDisco::ControladorDisco(Disco& d) {
	disco = &d;

	tamBloque = disco->obtenerTamBloque();
	numBloques = disco->obtenerNumBloques();
}

long ControladorDisco::_leer(char *buffer, long bloque) {
	return disco->leer(buffer, bloque);
}

long ControladorDisco::_escribir(char *buffer, long bloque) {
	return disco->escribir(buffer, bloque);
}
