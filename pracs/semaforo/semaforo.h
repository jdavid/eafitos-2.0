//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/semaforo.h
//	Definición de los semáforos.
// --------------------------------------------------------------------

#ifndef _SEMAFOROS
#define _SEMAFOROS

#include <cola.h>


#define N_SEMAFOROS 20


class Semaforo : public Cola {
	long cerradura;
public:
	Semaforo() { cerradura = -1; }

	long libre();
	long es(long c);
	void reservar(long c);
	void liberar();
};



class Semaforos : public Cola {
	Semaforo semaforos[N_SEMAFOROS];
public:
	void reservar(long cerradura);
	void liberar(long cerradura);
};



#endif
