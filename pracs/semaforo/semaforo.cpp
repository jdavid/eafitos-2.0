//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/semaforo.cpp
//	Implementaci�n de los sem�foros.
// --------------------------------------------------------------------

#include <semaforo.h>
#include <nucleo.h>


long Semaforo::libre() {
	if (cerradura<0)
		return 1;
	return 0;
}

long Semaforo::es(long c) {
	if (cerradura==c)
		return 1;
	return 0;
}

void Semaforo::reservar(long c) {
	if (cerradura<0) {
		cerradura = c;
		return;
	}

	if (cerradura==c) {
		// Transici�n de EJECUCI�N a SUSPENDIDO
		long hilo = Nucleo::manejadorProcesos.extraer();
		hilos[hilo].suspender();
		insertar(hilo);
		return;
	}
		
	throw "la cerradura no se corresponde";
}

void Semaforo::liberar() {
	if (libre())
		throw "este sem�faro no est� reservado";

	try {
		// Transici�n de SUSPENDIDO a LISTO
		long hilo = extraer();
		hilos[hilo].reactivar(0);
		Nucleo::manejadorProcesos.insertar(hilo);
	}
	catch(...) {
		cerradura = -1;
	}
}


void Semaforos::reservar(long cerradura) {
	for (long i=0; i<N_SEMAFOROS; i++) {
		if (semaforos[i].es(cerradura)) {
			semaforos[i].reservar(cerradura);
			return;
		}
	}

	for (long i=0; i<N_SEMAFOROS; i++) {
		if (semaforos[i].libre()) {
			semaforos[i].reservar(cerradura);
			return;
		}
	}

	throw "no quedan cerraduras libres";
}

void Semaforos::liberar(long cerradura) {
	for (long i=0; i<N_SEMAFOROS; i++) {
		if (semaforos[i].es(cerradura)) {
			semaforos[i].liberar();
			return;
		}
	}

	throw "no quedan cerraduras libres";
}
