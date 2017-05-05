//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/memfis.h
//	Definici�n de la memoria f�sica de la maquina virtual.
// --------------------------------------------------------------------


#ifndef _MEMORIA_FISICA
#define _MEMORIA_FISICA


#define TAM_MEMORIA 8192


class MemFis {
	char memoria[TAM_MEMORIA];
public:
	void escribirByte(char dato, long direccion);
	void escribirPalabra(long dato, long direccion);
	char leerByte(long direccion);
	long leerPalabra(long direccion);
};

#endif
