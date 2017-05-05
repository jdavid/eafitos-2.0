//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/controla.h
//	Definici�n de un controlador de dispostivo.
// --------------------------------------------------------------------

#ifndef _CONTROLADOR_DISPOSITIVO
#define _CONTROLADOR_DISPOSITIVO


#include <peticion.h>
#include <cola.h>


class Controlador : public Cola {
protected:
	// M�todo utilizado para registrar una petici�n y pasar el hilo a
	//   suspendido.
	long nuevaPeticion();

	// Una vez pasado un tiempo se llama a este m�todo para resolver
	//   la petici�n.
	virtual long ejecutarPeticion() = 0;
public:
	// M�todo principal, encargado de gestionar la cola de hilos
	void planificador();
};


#endif
