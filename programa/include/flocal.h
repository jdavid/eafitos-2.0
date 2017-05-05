//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/flocal.h
//	Definici�n de un fichero local. Todos los procesos tienen
//	varios.
// --------------------------------------------------------------------

#ifndef _FICHERO_LOCAL
#define _FICHERO_LOCAL


// Clase que define los ficheros locales de los procesos, es decir, las
//   entradas de las tablas de ficheros asociadas a los procesos.
class FicheroLocal {
	// Indica si se trata de un fichero normal o de un dispositivo o si
	//   la entrada est� libre
	enum Tipo {LIBRE, FICHERO, DISPOSITIVO};
	Tipo tipo;

	// Si es de tipo dispositivo, este atributo apunta a su controlador
	class ControladorCaracter* controlador;

	// Si es un ficero normal...
	long id;	// Identifica al fichero global asociado
	long posicion;	// La posici�n dentro del fichero
public:
	FicheroLocal();

	// Para saber si la entrada
	long libre();

	// M�todos para las llamadas al sistema
	long crear(long unidad, long pwd, char* nombre, long tipo);
	long abrir(long unidad, long pwd, char* nombre);
	void cerrar();
	long leer(void* destino, long nBytes);
	long escribir(void* fuente, long nBytes);
	long saltar(long pos, long desde);
};


#endif
