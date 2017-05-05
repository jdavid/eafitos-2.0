//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/peticion.h
//	Definición de una petición de entrada/salida.
// --------------------------------------------------------------------

#ifndef _PETICION_ES
#define _PETICION_ES


class Peticion {
public:
	// Distintos tipos de peticiones posibles
	enum Tipo {LEER_CARACTER, ESCRIBIR_CARACTER, LEER_BLOQUE,
		ESCRIBIR_BLOQUE};

	Tipo codigo;

	// Atributos que guardan información sobre la petición
	long dato;
	char* datos;
};


#endif
