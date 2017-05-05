//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/bloque.cpp
//	Implementaci�n de la lista de controladores de tipo bloque.
// --------------------------------------------------------------------

#include <bloque.h>
#include <mv.h>
#include <cdisco.h>


void DispositivosBloque::iniciar() {
	for (long i=0; i<NUM_DISPOS_BLOQUE; i++) {
		dispositivos[i] = NULL;
		try {
			iniDisco(i);
		}
		catch(...) {}
	}
}


void DispositivosBloque::iniDisco(long id) {
	Disco& disco = MaqVirtual::disco(id);
	dispositivos[id] = new ControladorDisco(disco);
}

ControladorBloque* DispositivosBloque::obtenerControlador(long id) {
	if (id<0 || id>=NUM_DISPOS_BLOQUE)
		throw "n�mero de dispositivo fuera de rango";
	return dispositivos[id];
}


long DispositivosBloque::gestionarColas() {
	long res = 0;

	// Recorre la tabla de controladores de tipo bloque
	for (long i=0; i<NUM_DISPOS_BLOQUE; i++)
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
