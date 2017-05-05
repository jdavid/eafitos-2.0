//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/memoria.cpp
//	Implementaci�n del manejador de memoria.
// --------------------------------------------------------------------

#include <memoria.h>
#include <mv.h>


long ManejadorMemoria::asignar(long cantidad) {
	return MaqVirtual::mmu.asignar(cantidad);
}

long ManejadorMemoria::reservar(long segmento) {
	return MaqVirtual::mmu.reservar(segmento);
}

long ManejadorMemoria::liberar(long segmento) {
	return MaqVirtual::mmu.liberar(segmento);
}


void ManejadorMemoria::aUsuario(long segmento, long dtno, char* fte, long nBytes) {
	for (long i=0; i<nBytes; i++)
		MaqVirtual::cpu.escribirByte(segmento, dtno+i, fte[i]);
}

void ManejadorMemoria::deUsuario(long segmento, long fte, char* dtno, long nBytes) {
	for (long i=0; i<nBytes; i++)
		dtno[i] = MaqVirtual::cpu.leerByte(segmento, fte+i);
}
