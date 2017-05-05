//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	eafitos2.cc
//	Este fichero tan solo contiene la función main.
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
