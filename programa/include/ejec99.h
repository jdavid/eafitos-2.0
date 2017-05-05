//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/ejec99.h
//	Definici�n del formato ejecutable 99
// --------------------------------------------------------------------

#ifndef _EJECUTABLE_99
#define _EJECUTABLE_99

#include <ejec.h>

// N�mero m�gico, identifica al fichero
#define ID_EJECUTABLE_99 2323

// Formato de fichero ejecutable de Eafitos2.
// Cabecera:
//	long id
//	long inicioPrograma
//	long tamPila
class Ejecutable99 : public Ejecutable {
public:
	// Carga un fichero ejecutable con el formato arriba indicado
	void ejecutar(long fichero, Contexto& contexto);
};


#endif
