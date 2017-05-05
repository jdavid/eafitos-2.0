//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/cola.h
//	Definición de las colas de hilos
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
