//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/peticion.h
//	Definici�n de una petici�n de entrada/salida.
// --------------------------------------------------------------------

#ifndef _PETICION_ES
#define _PETICION_ES


class Peticion {
public:
	// Distintos tipos de peticiones posibles
	enum Tipo {LEER_CARACTER, ESCRIBIR_CARACTER, LEER_BLOQUE,
		ESCRIBIR_BLOQUE};

	Tipo codigo;

	// Atributos que guardan informaci�n sobre la petici�n
	long dato;
	char* datos;
};


#endif
