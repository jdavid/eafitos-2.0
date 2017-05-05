//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/controla.cpp
//	Implementación de un controlador de dispositivo.
// --------------------------------------------------------------------

#include <controla.h>
#include <nucleo.h>


long Controlador::nuevaPeticion() {
	// Transición EJECUCION -> SUSPENDIDO
	long hilo = Nucleo::manejadorProcesos.extraer();
	hilos[hilo].suspender();
	insertar(hilo);

	// Si no habían hilos en SUSPENDIDO inicializa el contador de tiempo 
	if (actual()==hilo)
		tiempo = 10;

	return hilo;
}


void Controlador::planificador() {
	long hilo = actual();
	if (--tiempo==0) {
		// Ejecutamos la petición pendiente
		long resultado = ejecutarPeticion();

		// Transición SUSPENDIDO -> LISTO
		extraer();
		hilos[hilo].reactivar(resultado);
		Nucleo::manejadorProcesos.insertar(hilo);

		// Inicializo el contador.
		// Esto debería depender del dispositivo concreto.
		tiempo = 10;
	}
}
