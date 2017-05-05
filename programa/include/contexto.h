//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/contexto.h
//	Definici�n del contexto de ejecuci�n de un hilo.
// --------------------------------------------------------------------

#ifndef _CONTEXTO
#define _CONTEXTO


#define NUM_REGISTROS 16


// La clase Contexto define lo que se conoce como el contexto de ejecuci�n de
// un hilo, es decir, los registros basicos de la CPU que diferencian a un
// hilo de otro.
class Contexto {
public:
	long pc;			// Contador de programa
	long sp;			// Puntero de pila

	long registros[NUM_REGISTROS];	// registros generales

	long codigo;	// Segemento de c�digo y datos est�ticos
	long pila;	// Segmento de pila
};


#endif
