//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/controla.cpp
//	Implementaci�n de un controlador de dispositivo.
// --------------------------------------------------------------------

#include <controla.h>
#include <nucleo.h>


long Controlador::nuevaPeticion() {
	// Transici�n EJECUCION -> SUSPENDIDO
	long hilo = Nucleo::manejadorProcesos.extraer();
	hilos[hilo].suspender();
	insertar(hilo);

	// Si no hab�an hilos en SUSPENDIDO inicializa el contador de tiempo 
	if (actual()==hilo)
		tiempo = 10;

	return hilo;
}


void Controlador::planificador() {
	long hilo = actual();
	if (--tiempo==0) {
		// Ejecutamos la petici�n pendiente
		long resultado = ejecutarPeticion();

		// Transici�n SUSPENDIDO -> LISTO
		extraer();
		hilos[hilo].reactivar(resultado);
		Nucleo::manejadorProcesos.insertar(hilo);

		// Inicializo el contador.
		// Esto deber�a depender del dispositivo concreto.
		tiempo = 10;
	}
}
