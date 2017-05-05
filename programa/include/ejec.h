//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/ejec.h
//	Definición de la clase base Ejecutable
// --------------------------------------------------------------------

#ifndef _EJECUTABLE
#define _EJECUTABLE

#include <contexto.h>

// Clase base para los formatos de ficheros ejecutables
class Ejecutable {
public:
	// Método para cargar un fichero ejecutable
	virtual void ejecutar(long fichero, Contexto& contexto) = 0;
};


#endif
