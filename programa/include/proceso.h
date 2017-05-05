//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/proceso.h
//	Definici�n de un proceso.
// --------------------------------------------------------------------


#ifndef _PROCESO
#define _PROCESO


#include <flocal.h>

#define N_F_LOCALES 10


class Proceso {
	long uso;

	FicheroLocal ficherosLocales[N_F_LOCALES];
	long unidad;
	long directorio;
public:
	Proceso();

	// Indica si este proceso existe o no.
	long libre();

	// Inicia y termina este proceso.
	long iniciar(long u = 0, long d = 0);
	long terminar();

	// M�todos que implementan en parte diversas llamadas al sistema.
	//   Para cambiar la unidad y el directorio actuales
	void cambiarUnidad(long num);
	void cambiarDirectorio(char* nombre);

	//   Del sistema de ficheros
	long crear(char* nombre, long tipo);
	long abrir(char* nombre);
	void cerrar(long id);
	long leer(long id, void* destino, long nBytes);
	long escribir(long id, void* fuente, long nBytes);
	long saltar(long id, long posicion, long desde);
	void borrar(char* nombre);

	//   Para ejecutar programas
	void ejecutar(char* nombre);

	//   Operaciones sobre la entrada/salida est�ndar
	long leerCaracter();
	void imprimirCaracter(char c);
};


#endif
