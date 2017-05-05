//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	mv/mv.cpp
//	Implementaci�n de la m�quina virtual.
// --------------------------------------------------------------------

#include <mv.h>


MemFis MaqVirtual::memoria;
MMU MaqVirtual::mmu;
CPU MaqVirtual::cpu;
Teclado MaqVirtual::teclado;
Pantalla MaqVirtual::pantalla;
Disco* MaqVirtual::discos[NUM_DISCOS];


void MaqVirtual::crearDisco(char id, long tamBloque, long numBloques) {
	if (id<0 || id>NUM_DISCOS)
		throw "n�mero de disco no v�lido";

	if (discos[id])
		throw "ese disco ya existe";

	discos[id] = new Disco(id+'0', tamBloque, numBloques);
}

Disco& MaqVirtual::disco(long id) {
	if (id<0 || id>=NUM_DISCOS)
		throw "ese disco no existe";

	if (discos[id]==NULL)
		throw "ese disco no existe";

	return *(discos[id]);
}


void MaqVirtual::iniciar() {
	// Inicializar los discos
	for (long i=0; i<NUM_DISCOS; i++) {
		discos[i] = NULL;
		try {
			discos[i] = new Disco(i+'0');
		}
		catch(...) {}
	}
}

void MaqVirtual::terminar() {
	// Limpiar la tabla de discos
	for (long i=0; i<5; i++)
		if (discos[i])
			delete discos[i];
}
