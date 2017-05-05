//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/controla.h
//	Definición de un controlador de dispostivo.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_DISPOSITIVO
#define _CONTROLADOR_DISPOSITIVO


#include <peticion.h>
#include <cola.h>


class Controlador : public Cola {
protected:
	// Método utilizado para registrar una petición y pasar el hilo a
	//   suspendido.
	long nuevaPeticion();

	// Una vez pasado un tiempo se llama a este método para resolver
	//   la petición.
	virtual long ejecutarPeticion() = 0;
public:
	// Método principal, encargado de gestionar la cola de hilos
	void planificador();
};


#endif
