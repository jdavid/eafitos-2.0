//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/cola.cpp
//	Implementación de la cola de hilos que esperan por un recurso.
// --------------------------------------------------------------------

#include <cola.h>


Hilo Cola::hilos[N_HILOS];


Cola::Cola() {
	primero = -1;
}


// Devuelve el identificador del hilo que es cabeza de la cola
long Cola::actual() {
	if (primero<0)
		throw "no hay ningún hilo en la cola";

	return primero;
}


// Inserta un hilo en el final de la cola
long Cola::insertar(long hilo) {
	if (primero == -1) {
		primero = hilo;
		hilos[hilo].setAnt(hilo);
		hilos[hilo].setSig(hilo);
	} else {
		hilos[hilos[primero].getAnt()].setSig(hilo);
		hilos[hilo].setAnt(hilos[primero].getAnt());
		hilos[primero].setAnt(hilo);
		hilos[hilo].setSig(primero); 
	}

        return hilo;
}

// Extrae un hilo de la cola
long Cola::extraer(long hilo) {
	if (primero == -1)
		throw "no hay ningún hilo en la cola";

	// Por defecto (si no se específica el número de hilo) se elimina el
	//   hilo de la cabeza
	if (hilo==-1)
		hilo = primero;

	if (hilos[hilo].getSig() == primero)
		primero = -1;
	else {
		hilos[hilos[hilo].getAnt()].setSig(hilos[hilo].getSig());
		hilos[hilos[hilo].getSig()].setAnt(hilos[hilo].getAnt());
		if (hilo == primero)
			primero = hilos[hilo].getSig();
	}

	return hilo;
}
