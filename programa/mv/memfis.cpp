//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	mv/memfis.cpp
//	Implementación de la memoria física.
// --------------------------------------------------------------------

#include <memfis.h>


void MemFis::escribirByte(char dato, long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA)
		throw "dirección fuera de rango";

	memoria[direccion] = dato;
}

void MemFis::escribirPalabra(long dato, long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA-3)
		throw "dirección fuera de rango";

	*((long*)(memoria + direccion)) = dato;
}

char MemFis::leerByte(long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA)
		throw "dirección fuera de rango";

	return memoria[direccion];
}

long MemFis::leerPalabra(long direccion) {
	if (direccion<0 || direccion>=TAM_MEMORIA-3)
		throw "dirección fuera de rango";

	return *((long*)(memoria + direccion));
}
