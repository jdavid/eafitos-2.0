//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/cdisco.h
//	Definición del controlador del disco.
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
