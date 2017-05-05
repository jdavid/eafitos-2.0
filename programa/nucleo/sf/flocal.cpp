//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/sf/flocal.cpp
//	Implementación de los ficheros locales de cada proceso.
// --------------------------------------------------------------------

#include <flocal.h>
#include <nucleo.h>


FicheroLocal::FicheroLocal() {
	tipo = LIBRE;
}


long FicheroLocal::libre() {
	if (tipo==LIBRE)
		return 1;
	return 0;
}


long FicheroLocal::crear(long unidad, long pwd, char* nombre, long tipo) {
	// Comprueba que la entrada esté libre
	if (tipo!=LIBRE)
		throw "no puedes abrir un fichero que ya está abierto";

	// Crea el fichero
	id = Nucleo::sfv.crear(unidad, pwd, nombre, tipo);
	FicheroLocal::tipo = FICHERO;

	// Inicialmente la posición del fichero es cero
	posicion = 0;

	// Devuelve el identificador global del fichero
	return id;
}

long FicheroLocal::abrir(long unidad, long pwd, char* nombre) {
	// Comprueba que la entrada esté libre
	if (tipo!=LIBRE)
		throw "no puedes abrir un fichero que ya está abierto";

	// Busca un controlador de dispositivo identificado con "nombre"
	controlador = Nucleo::dispositivosCaracter.obtenerControlador(nombre);
	if (controlador) {
		// El fichero es en realidad un controlador
		tipo = DISPOSITIVO;
		return -1;
	} else {
		// Es un fichero normal, lo abre
		tipo = FICHERO;
		posicion = 0;
		return Nucleo::sfv.abrir(unidad, pwd, nombre);
	}
}

void FicheroLocal::cerrar() {
	if (tipo==LIBRE)
		throw "no puedes cerrar un fichero que no está abierto";

	// Si es un fichero normal lo cierra
	if (tipo==FICHERO)
		Nucleo::sfv.cerrar(id);

	tipo = LIBRE;
}

long FicheroLocal::leer(void* destino, long nBytes) {
	if (tipo==LIBRE)
		throw "no puedes leer de un fichero que no está abierto";

	// Realiza una operación u otra dependiendo del tipo de fichero
	if (tipo==FICHERO) {
		// Si es un fichero normal leemos de él llamando a SFV
		Nucleo::sfv.saltar(id, posicion, PRINCIPIO);
		nBytes = Nucleo::sfv.leer(id, destino, nBytes);
		posicion += nBytes;
	}
	else if (tipo==DISPOSITIVO) 
		// Si es un fichero de tipo dispositivo leemos con el
		//   método ControladorCaracter::leerCaracter
		for (long i=0; i<nBytes; i++)
			((char*)destino)[i] = controlador->leerCaracter();

	// Devolvemos el número de bytes leidos
	return nBytes;
}

long FicheroLocal::escribir(void* fuente, long nBytes) {
	if (tipo==LIBRE)
		throw "no puedes escribir en un fichero que no está abierto";

	// Realiza una operación u otra dependiendo del tipo de fichero
	if (tipo==FICHERO) {
		// Si es un fichero normal escribimos en él llamando a SFV
		Nucleo::sfv.saltar(id, posicion, PRINCIPIO);
		nBytes = Nucleo::sfv.escribir(id, fuente, nBytes);
		posicion += nBytes;
	}
	else if (tipo==DISPOSITIVO) 
		// Si es un fichero de tipo dispositivo escribimos con el
		//   método ControladorCaracter::leerCaracter
		for (long i=0; i<nBytes; i++)
			 controlador->escribirCaracter(((char*)fuente)[i]);

	// Devolvemos el número de bytes escritos
	return nBytes;
}

long FicheroLocal::saltar(long pos, long desde) {
	if (tipo==LIBRE)
		throw "no puedes saltar en un fichero que no está abierto";

	// Si es un fichero normal cambiamos la posición (si es de tipo
	//   dispositivo no tiene sentido cambiarla)
	if (tipo==FICHERO)
		posicion = Nucleo::sfv.saltar(id, pos, desde);

	return posicion;
}
