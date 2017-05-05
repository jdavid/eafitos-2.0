//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/mv.h
//	Definición de la máquina virtual.
// --------------------------------------------------------------------

#ifndef _MAQUINA_VIRTUAL
#define _MAQUINA_VIRTUAL

#include <memfis.h>
#include <mmu.h>
#include <cpu.h>
#include <teclado.h>
#include <pantalla.h>
#include <disco.h>


#define NUM_DISCOS 5


class MaqVirtual {
public:
	static MemFis memoria;
	static MMU mmu;
	static CPU cpu;
	static Teclado teclado;
	static Pantalla pantalla;
private:
	static Disco* discos[NUM_DISCOS];

public:
	// Método para crear nuevos discos
	static void crearDisco(char id, long tamBloque, long numBloques);

	// Devuelve una referencia al disco especificado
	static Disco& disco(long id);

	static void iniciar();
	static void terminar();
};


#endif
