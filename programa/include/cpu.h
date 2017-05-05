//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/cpu.h
//	Definición de la unidad central de proceso (CPU).
// --------------------------------------------------------------------

#ifndef _CPU
#define _CPU


#include <contexto.h>


// Juego de instrucciones
enum Instrucciones {SUMAR, RESTAR, AND, OR, COPIAR, NOT,
	CARGAR_I, CARGAR32, CARGAR8, GUARDAR_I, GUARDAR32, GUARDAR8,
	APILAR, DESAPILAR,
	SALTAR, SALTAR0, SALTARP, SALTARN, NOP, SER_SIS};


class CPU {
	Contexto contexto;
public:
	Contexto& obtenerContexto()
		{ return contexto; }

	void ponerContexto(Contexto& c)
		{ contexto = c; }

	// Métodos de Lectura/Escritura en memoria
	void escribirByte(long segmento, long direccion, char dato);
	void escribirPalabra(long segmento, long direccion, long dato);
	char leerByte(long segmento, long direccion);
	long leerPalabra(long segmento, long direccion);

	// Métodos de lectura de operandos e instrucciones y de acceso a pila
	char leerOpByte();
	long leerOpPalabra();
	void apilar(long dato);
	long desapilar();

	void ejecutarPaso();	// Ejecuta una instrucción
	void ejecutar();	// Ejecuta instrucciones hasta que se acaban
};


#endif
