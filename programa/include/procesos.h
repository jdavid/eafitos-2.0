//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/procesos.h
//	Definici�n del manejador de hilos y procesos.
// --------------------------------------------------------------------

#ifndef _PROCESOS
#define _PROCESOS


#define N_PROCESOS 10
#define N_EJECUTABLES 5
#define TAJADA 100


#include <cola.h>
#include <proceso.h>
#include <ejec.h>


class ManejadorProcesos : public Cola {
	// Tabla de procesos
	Proceso procesos[N_PROCESOS];

	// Tabla de formatos de ficheros ejecutables
	Ejecutable* ejecutables[N_EJECUTABLES];
public:
	void iniciar();
	void terminar();

	// Devuelve el proceso actual
	Proceso* procesoEjecucion();

	// Metodos que implementan las correspondientes llamadas al sistema.
	long crearHilo(long direccion);
	void terminarHilo(long id=-1);

	// M�todo que ejecuta un programa.
	long ejecutar(long unidad, long pwd, char* nombre);

	// Metodo encargado de la politica de paso de un hilo de listo a
	// ejecucion y viceversa.
	void planificador();
};


#endif
