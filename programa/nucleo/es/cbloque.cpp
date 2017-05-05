//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/cbloque.cpp
//	Implmentaci�n de un controlador de tipo bloque.
// --------------------------------------------------------------------

#include <cbloque.h>


long ControladorBloque::ejecutarPeticion() {
	// Obtiene la petici�n
	Peticion& peticion = hilos[actual()].obtenerPeticion();

	// Ejecuta la petici�n
	if (peticion.codigo==Peticion::LEER_BLOQUE)
		_leer(peticion.datos, peticion.dato);
	else if (peticion.codigo==Peticion::ESCRIBIR_BLOQUE)
		_escribir(peticion.datos, peticion.dato);

	return 0;
}


// Para lee un bloque
long ControladorBloque::leer(char* datos, long bloque) {
	try {
		// Registra la petici�n
		long h = nuevaPeticion();
		hilos[h].ponerPeticion(Peticion::LEER_BLOQUE, bloque, datos);
		return 0;
	}
	catch(...) {
		return _leer(datos, bloque);
	}
}

// Para escribir un bloque
long ControladorBloque::escribir(char* datos, long bloque) {
	try {
		// Registra la petici�n
		long h = nuevaPeticion();
		hilos[h].ponerPeticion(Peticion::ESCRIBIR_BLOQUE, bloque,datos);
		return 0;
	}
	catch(...) {
		return _escribir(datos, bloque);
	}
}
