//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/contexto.h
//	Definición del contexto de ejecución de un hilo.
// --------------------------------------------------------------------

#ifndef _CONTEXTO
#define _CONTEXTO


#define NUM_REGISTROS 16


// La clase Contexto define lo que se conoce como el contexto de ejecución de
// un hilo, es decir, los registros basicos de la CPU que diferencian a un
// hilo de otro.
class Contexto {
public:
	long pc;			// Contador de programa
	long sp;			// Puntero de pila

	long registros[NUM_REGISTROS];	// registros generales

	long codigo;	// Segemento de código y datos estáticos
	long pila;	// Segmento de pila
};


#endif
