//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/cbloque.h
//	Definici�n del controlador de tipo bloque.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_DISPOSITIVO_BLOQUE
#define _CONTROLADOR_DISPOSITIVO_BLOQUE

#include <controla.h>


class ControladorBloque : public Controlador {
	// M�todo encargado de resolver las peticiones de entrada/salida
	long ejecutarPeticion();

protected:
	long tamBloque;
	long numBloques;
public:
	long obtenerTamBloque()		{ return tamBloque; }        	
	long obtenerNumBloques()	{ return numBloques; }

	// M�todos para realizar las peticiones de entrada/salida
	long leer(char *datos, long bloque);
	long escribir(char *datos, long bloque);

	// M�todos que ejecutan las peticiones
	virtual long _leer(char *datos, long bloque) = 0;
	virtual long _escribir(char *datos, long bloque) = 0;
};


#endif
