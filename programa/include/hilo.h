//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/hilo.h
//	Definición de un hilo.
// --------------------------------------------------------------------

#ifndef _HILO
#define _HILO


#include <contexto.h>
#include <peticion.h>


class Hilo {
	// Posibles estados de un hilo
	enum Estado {LIBRE, LISTO, EJECUCION, SUSPENDIDO};
	Estado estado;

	// Para guardar el contexto de ejecución del hilo cuando este no se
	//   encuentre en ejecución
	Contexto contexto;

	// Proceso asociado a este hilo
	class Proceso* proceso;

	// Guarda la petición de entrada/salida del hilo
	Peticion peticion;

	// Punteros a otros hilos, para las colas de hilos
	long siguiente, anterior;
public:
	Hilo();

	// Devuelve el proceso asociado a este hilo.
	Proceso* obtenerProceso();

	// Métodos para gestionar las poner/obtener las peticiones de E/S
	Peticion& obtenerPeticion();
	void ponerPeticion(Peticion::Tipo codigo);
	void ponerPeticion(Peticion::Tipo codigo, long dato);
	void ponerPeticion(Peticion::Tipo codigo, char* datos);
	void ponerPeticion(Peticion::Tipo codigo, long dato, char* datos);

	// Métodos para gestionar la cola (atributos anterior y siguiente)
	void setAnt(long ant)	{ anterior = ant; }
	void setSig(long sig)	{ siguiente = sig; }
	long getAnt()		{ return anterior; }
	long getSig()		{ return siguiente; }


	// Métodos de información sobre el estado
	char libre();
	char listo();

	// Métodos para los cambios de estado
	void iniciar(Proceso* proc, long direccion);	// Para la llamada al
							//  sistema CREAR_HILO.
	void iniciar(Proceso* proc, Contexto& ctx);	// Para la ejecución de
							//  programas.
	void ejecutar();
	void dormir();
	void suspender();
	void reactivar(long resultado);
	void terminar();
};


#endif
