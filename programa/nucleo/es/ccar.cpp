//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	nucleo/es/ccar.cpp
//	Implementaci�n de un controlador de tipo caracter.
// --------------------------------------------------------------------

#include <ccar.h>
#include <string.h>

// Ejecuta la petici�n del hilo que es cabeza de la cola
long ControladorCaracter::ejecutarPeticion() {
	// Obtiene la petici�n
	Peticion& peticion = hilos[actual()].obtenerPeticion();

	// Ejecuta la petici�n
	if (peticion.codigo==Peticion::LEER_CARACTER)
		return _leerCaracter();
	else if (peticion.codigo==Peticion::ESCRIBIR_CARACTER)
		_escribirCaracter(peticion.dato);

	return 0;
}

// Inicializa el controlador de car�cter asignandole un identificador (nombre)
ControladorCaracter::ControladorCaracter(char* n) {
	nombre = new char[strlen(n)+1];
	strcpy(nombre, n);
}

ControladorCaracter::~ControladorCaracter() {
	if (nombre)
		delete nombre;
}


long ControladorCaracter::es(char* n) {
	if (strcmp(nombre, n) == 0)
		return 1;
	return 0;
}

// Para leer un car�cter
char ControladorCaracter::leerCaracter() {
	try {
		// Registra la petici�n
		long h = nuevaPeticion();
		hilos[h].ponerPeticion(Peticion::LEER_CARACTER);
		return 0;
	}
	catch(...) {
		return _leerCaracter();
	}
}

// Para imiprimir un car�cter
void ControladorCaracter::escribirCaracter(char caracter) {
	try {
		// Registra la petici�n
		long h = nuevaPeticion();
		hilos[h].ponerPeticion(Peticion::ESCRIBIR_CARACTER, caracter);
	}
	catch(...) {
		_escribirCaracter(caracter);
	}
}
