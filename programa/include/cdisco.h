//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/cdisco.h
//	Definici�n del controlador del disco.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_DISCO
#define _CONTROLADOR_DISCO

#include <cbloque.h>
#include <disco.h>


class ControladorDisco : public ControladorBloque {
	Disco* disco;
public:
	ControladorDisco(Disco& d);

	long _leer(char *buffer, long bloque);
	long _escribir(char *buffer, long bloque);
};



#endif
