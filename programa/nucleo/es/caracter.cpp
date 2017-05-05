//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/caracter.cpp
//	Implementaci�n de la lista de controladores de tipo caracter.
// --------------------------------------------------------------------

#include <caracter.h>
#include <cteclado.h>
#include <cpantall.h>


void DispositivosCaracter::iniciar() {
	// Teclado (0) y pantalla (1)
	dispositivos[0] = new ControladorTeclado("--teclado--");
	dispositivos[1] = new ControladorPantalla("--pantalla--");

	// Ya no quedan m�s dispositivos.
	for (long i=2; i<NUM_DISPOS_CARACTER; i++)
		dispositivos[i] = 0;
}


ControladorCaracter* DispositivosCaracter::obtenerControlador(char* nombre) {
	for (long i=0; i<NUM_DISPOS_CARACTER; i++)
		if (dispositivos[i])
			if (dispositivos[i]->es(nombre))
				return dispositivos[i];

	return 0;
}


long DispositivosCaracter::gestionarColas() {
	long res = 0;

	// Recorre la tabla de controladores de tipo car�cter
	for (long i=0; i<NUM_DISPOS_CARACTER; i++)
		// Si el controlador existe...
		if (dispositivos[i]) {
			try {
				// ...llama a su planificador
				dispositivos[i]->planificador();
				res = 1;
			}
			catch(...) {}
		}
	return res;
}
