//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	eafitos2.cc
//	Este fichero tan solo contiene la funci�n main.
// --------------------------------------------------------------------


#include <mv.h>
#include <nucleo.h>
#include <entorno.h>


int main(void)
{
	MaqVirtual::iniciar();
	Nucleo::iniciar();
	Entorno::iniciar();
	Nucleo::terminar();
	MaqVirtual::terminar();

	return 0;
}
