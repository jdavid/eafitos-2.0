//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/ic.h
//	Definici�n del longerprete de comandos.
// --------------------------------------------------------------------

#ifndef _INTERPRETE_COMANDOS
#define _INTERPRETE_COMANDOS


class InterpreteComandos {
	char linea[100];	// Almacena la linea que se lee del teclado

	long unidad;		// Identifica la unidad de disco actual
	long directorio;	// Identifica el directorio actual
public:
	void iniciar();
};



#endif
