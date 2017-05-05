//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/ejec99.h
//	Definición del formato ejecutable 99
// --------------------------------------------------------------------

#ifndef _EJECUTABLE_99
#define _EJECUTABLE_99

#include <ejec.h>

// Número mágico, identifica al fichero
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
