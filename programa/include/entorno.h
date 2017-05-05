//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	entorno.h
//	Este fichero tene la definición de Eafitos.
// --------------------------------------------------------------------


#ifndef _ENTORNO
#define _ENTORNO


#include <ic.h>
#include <ensambla.h>


// Clase que contiene al intérprete de comandos, al compilador y que implementa
//   los menús de gestión de discos.
class Entorno {
public:
	static InterpreteComandos interpreteComandos;
	static Ensamblador ensamblador;

	static void crearDiscoNuevo();
	static void formatearDisco();
	static void iniciar();
};


#endif
