//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/cola.h
//	Definici�n de las colas de hilos
// --------------------------------------------------------------------

#ifndef _COLA
#define _COLA


#include <hilo.h>

#define N_HILOS 20

class Cola {
protected:
	static Hilo hilos[N_HILOS];
	long primero;
	long tiempo;
public:
	Cola();

	long actual();
	long insertar(long hilo);
	long extraer(long hilo = -1);
};


#endif
