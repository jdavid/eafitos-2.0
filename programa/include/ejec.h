//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/ejec.h
//	Definici�n de la clase base Ejecutable
// --------------------------------------------------------------------

#ifndef _EJECUTABLE
#define _EJECUTABLE

#include <contexto.h>

// Clase base para los formatos de ficheros ejecutables
class Ejecutable {
public:
	// M�todo para cargar un fichero ejecutable
	virtual void ejecutar(long fichero, Contexto& contexto) = 0;
};


#endif
