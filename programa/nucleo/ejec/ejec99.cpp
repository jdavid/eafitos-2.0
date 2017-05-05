//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/ejec/ejec99.cpp
//	Implementación del formato ejecutable 99
// --------------------------------------------------------------------

#include <ejec99.h>
#include <nucleo.h>


void Ejecutable99::ejecutar(long fichero, Contexto& contexto) {
	SFV& sfv = Nucleo::sfv;
	ManejadorMemoria& manejadorMemoria = Nucleo::manejadorMemoria;

	// Posiciona el puntero dentro del fichero para leer su cabecera
	sfv.saltar(fichero, 0, PRINCIPIO);

	// Comprueba que el fichero corresponde a este formato.
	long id;
	sfv.leer(fichero, &id, sizeof(id));
	if (id!=ID_EJECUTABLE_99)
		throw "formato de ejecutable desconocido";

	// Lee el resto de la cabecera:
	//   el inicio del programa
	sfv.leer(fichero, &(contexto.pc), sizeof(long));
	//   el tamano de la pila
	long tamPila;
	sfv.leer(fichero, &tamPila, sizeof(tamPila));

	// Calcula el tamano de la imagen
	long tamImagen = sfv.obtenerTamano(fichero) - 3*sizeof(long);

	// Inicializa la pila
	contexto.pila = manejadorMemoria.asignar(tamPila);
	contexto.sp = 0;

	// Lee la imagen del ejecutable
	char* buffer = new char[tamImagen];
	sfv.leer(fichero, buffer, tamImagen);

	// Inicializa el segmento de código y datos estáticos
	contexto.codigo = manejadorMemoria.asignar(tamImagen);
	manejadorMemoria.aUsuario(contexto.codigo, 0, buffer, tamImagen);
}
