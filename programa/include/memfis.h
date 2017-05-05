//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/memfis.h
//	Definición de la memoria física de la maquina virtual.
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
