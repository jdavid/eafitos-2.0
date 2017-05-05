//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	entorno.h
//	Este fichero tene la definici�n de Eafitos.
// --------------------------------------------------------------------


#ifndef _ENTORNO
#define _ENTORNO


#include <ic.h>
#include <ensambla.h>


// Clase que contiene al int�rprete de comandos, al compilador y que implementa
//   los men�s de gesti�n de discos.
class Entorno {
public:
	static InterpreteComandos interpreteComandos;
	static Ensamblador ensamblador;

	static void crearDiscoNuevo();
	static void formatearDisco();
	static void iniciar();
};


#endif
