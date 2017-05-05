//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	mv/memfis.cpp
//	Implementaci�n de la memoria f�sica.
// --------------------------------------------------------------------

#include <memfis.h>


void MemFis::escribirByte(char dato, long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA)
		throw "direcci�n fuera de rango";

	memoria[direccion] = dato;
}

void MemFis::escribirPalabra(long dato, long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA-3)
		throw "direcci�n fuera de rango";

	*((long*)(memoria + direccion)) = dato;
}

char MemFis::leerByte(long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA)
		throw "direcci�n fuera de rango";

	return memoria[direccion];
}

long MemFis::leerPalabra(long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA-3)
		throw "direcci�n fuera de rango";

	return *((long*)(memoria + direccion));
}
