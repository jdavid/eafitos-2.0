//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/sf/efichero.cpp
//	Implementación del sistema de ficheros EAFIT. La parte que
//	entiende de los ficheros.
// --------------------------------------------------------------------

#include <string.h>
#include <eafitsf.h>

// Devuelve el número de bloque que corresponde a la posición actual dentro
//   del fichero, si es final de fichero genera un error
long EFichero::obtenerNBloque() {
	// Si es final de fichero genera error
	if (finalFichero())
		throw "final de fichero";

	// Calcula el número de bloque relativo del fichero
	long n = posicion/tamBloque;

	// Devuelve el número de bloque en el sistema de ficheros
	if (n<NBDIRECTOS)
		return inodo.bloquesDirectos[n];

        return bloquesIndirectos[n-NBDIRECTOS];
}

// Devuelve el número de bloque que corresponde a la posición actual dentro
//   del fichero, si es final de fichero reserva un byte más
long EFichero::reservarBloque() {
	// Si es final de fichero reserva memoria
	if (finalFichero()) {
		// Incrementa el tamaño del fichero
		inodo.tamano++;

		// Si ya no queda espacio en este bloque reservamos otro
		if (posicion % tamBloque == 0) {
			// Reservamos un y obtenemos su número
			long nBloque = sf->reservarBloque();

			// Calcula el número de bloque relativo del fichero
			long n = posicion/tamBloque;

			// Asocia al bloque reservado al fichero
			if (n<NBDIRECTOS)
				inodo.bloquesDirectos[n] = nBloque;
			else {
				bloquesIndirectos[n-NBDIRECTOS] = nBloque;
				// Si es necesario reservamos un bloque para
				//   los bloques indirectos 
				if (n==NBDIRECTOS)
					inodo.bloquesIndirectos =
						sf->reservarBloque();
			}
		}
	}

	// Llama a obtenerNBloque para saber el número de bloque actual
	return obtenerNBloque();
}


EFichero::EFichero(EafitSF* sf, long nInodo, InodoESF& inodo) : Fichero(sf) {
	EFichero::sf = sf;
	EFichero::nInodo = nInodo;
	EFichero::inodo = inodo;

	// Reserva memoria para los bloques indirectos, aunque no sea necesario
	bloquesIndirectos = (long*) new char[tamBloque];

	// Si es necesario lee el bloque de bloques indirectos
	if (inodo.tamano/tamBloque>=NBDIRECTOS)
		sf->leerBloque(inodo.bloquesIndirectos,(char*)bloquesIndirectos);
}

EFichero::~EFichero() {
	if (bloquesIndirectos)
        	delete bloquesIndirectos;
}


long EFichero::es(SF* s, long nI) {
	if (sf==s && nInodo==nI)
		return 1;
	return 0;
}

long EFichero::obtenerTamano() {
	return inodo.tamano;
}

long EFichero::finalFichero() {
	if (posicion>=inodo.tamano)
		return 1;
	return 0;
}


long EFichero::abrir() {
	uso++;
	return uso;
}

long EFichero::cerrar() {
	// Reduce el uso del fichero
	uso--;

	// Si ya no lo usa nadies
	if (uso==0) {
		// Escribe el inodo
		sf->escribirInodo(inodo, nInodo);

		// Si es necesario escribe el bloque de bloques indirectos
		if (inodo.tamano/tamBloque>NBDIRECTOS)
			sf->escribirBloque(inodo.bloquesIndirectos,
						(char*)bloquesIndirectos);

		// Libera la memoria reservada en el constructor
		delete buffer;
		buffer = NULL;
		delete bloquesIndirectos;
		bloquesIndirectos = NULL;
        }

	return uso;
}

void EFichero::borrar() {
	// Libera los bloques del fichero
	long i;
	for (i=0; i<inodo.tamano/tamBloque; i++) {
		if (i<NBDIRECTOS)
			sf->liberarBloque(inodo.bloquesDirectos[i]);
		else
			sf->liberarBloque(bloquesIndirectos[i-NBDIRECTOS]);
	}

	// Libera el bloque de bloques indirectos, si existe
	if (i>=NBDIRECTOS)
		sf->liberarBloque(inodo.bloquesIndirectos);
}

long EFichero::leer(char* dtno, long nDatos) {
	long i = 0;
	try {
		// Lee el bloque actual
		long nBloque = obtenerNBloque();
		sf->leerBloque(nBloque, buffer);

		// Lee todos los bytes que se puedan
		for (i=0; i<nDatos; i++) {
			// Si llegamos al final de fichero regresamos y
			//   devolvemos los bytes leidos
			if (posicion > inodo.tamano)
				return i;

			// Lee un byte y actualiza la posición
			dtno[i] = buffer[posicion%tamBloque];
			posicion++;

			// Si es necesario lee otro bloque
			if (posicion%tamBloque == 0) {
				nBloque = obtenerNBloque();
				sf->leerBloque(nBloque, buffer);
			}
		}
	}
	catch(...) {}

	// Devolvemos los bytes leidos
	return i;
}

long EFichero::escribir(char* fte, long nDatos) {
	long nBloque;

	for (long i=0; i<nDatos; i++) {
		// Lee el bloque actual (y si hace falta reserva uno)
		nBloque = reservarBloque();
		if (i==0 || posicion%tamBloque==0)
			sf->leerBloque(nBloque, buffer);

		// Escribe un byte en el buffer
		buffer[posicion%tamBloque] = fte[i];
		posicion++;

		// Cuando el buffer se llena, escribe el bloque
		if (posicion%tamBloque == 0)
			sf->escribirBloque(nBloque, buffer);
	}

	// Escribe el bloque
	sf->escribirBloque(nBloque, buffer);

	return nDatos;
}

long EFichero::saltar(long pos, long desde) {
	// Calcula la posicion nueva posicion
	if (desde==PRINCIPIO) {
		posicion = pos;
	} else if (desde==ACTUAL) {
		posicion += pos;
	} else {
		throw "posición inicial desconocida";
	}

	// Ajusta la posicion dentro de los limites del fichero
	if (posicion<0) {
		posicion = 0;
	} else if (posicion>inodo.tamano) {
		posicion = inodo.tamano;
	}

        return posicion;
}



void EDirectorio::crearEntrada(char* nombre, long inodo) {
	posicion = 0;
	EEntradaDirectorio entrada;

	// Busca una entrada libre
	while(leer((char*)&entrada, sizeof(entrada))==sizeof(entrada))
		if (entrada.nInodo < 0) {
			posicion -= sizeof(entrada);
			break;
		}

	// Crea la entrada
	strcpy(entrada.nombre, nombre);
	entrada.nInodo = inodo;

	// Escribe la entrada
	escribir((char*)&entrada, sizeof(entrada));
}

void EDirectorio::borrarEntrada(char* nombre) {
	posicion = 0;
	EEntradaDirectorio entrada;

	// Busca la entrada de fichero a borrar
	while(leer((char*)&entrada, sizeof(entrada))==sizeof(entrada))
		if (entrada.nInodo>=0 && strcmp(entrada.nombre, nombre)==0) {
			// Libera la entrada
			entrada.nInodo = -entrada.nInodo;
			posicion -= sizeof(entrada);
			escribir((char*)&entrada, sizeof(entrada));

			return;
		}

	throw "ese fichero no existe";
}


void EDirectorio::borrar() {
	EEntradaDirectorio entrada;
	posicion = sizeof(entrada);

	// Si el directorio no está vacio genera un error
	while(leer((char*)&entrada, sizeof(entrada))==sizeof(entrada))
		if (entrada.nInodo>=0)
			throw "el directorio no está vacio";

	EFichero::borrar();
}


long EDirectorio::idFichero(char* nombre) {
	posicion = 0;
	EEntradaDirectorio entrada;

	// Busca el fichero solicitado
	while(leer((char*)&entrada, sizeof(entrada))==sizeof(entrada))
		if (entrada.nInodo>=0 && strcmp(entrada.nombre, nombre)==0)
			// Devuelve el número de inodo del fichero
			return entrada.nInodo;

	throw "ese fichero no existe";
}

long EDirectorio::existeFichero(char* nombre) {
	posicion = 0;
	EEntradaDirectorio entrada;

	// Busca el fichero
	while(leer((char*)&entrada, sizeof(entrada))==sizeof(entrada))
		if (entrada.nInodo>=0 && strcmp(entrada.nombre, nombre)==0)
			// Encontrado, devuelve cierto
			return 1;

	// No se encontró, devuelve falso
	return 0;
}


Fichero* EDirectorio::crearFichero(char* nombre, char tipo) {
	// Comprueba que el fichero no exista
	if (existeFichero(nombre))
		throw "ese fichero ya existe";

	// Reserva un inodo para el fichero
	long nInodo = sf->reservarInodo();

	// Crea el inodo
	InodoESF inodo;
	inodo.tipo = tipo;
        inodo.tamano = 0;

	// Abre el fichero
	Fichero* fich;
	switch (tipo) {
		case NORMAL:
			fich = new EFichero(sf, nInodo, inodo);
			break;
		case DIRECTORIO:
			fich = new EDirectorio(sf, nInodo, inodo);
			// Crea la entrada al directorio padre
			((EDirectorio*)fich)->crearEntrada("..",
							EFichero::nInodo);
			break;
		default:
			throw "tipo de fichero desconocido";
	}

	// Crea la entrada en el directorio
	crearEntrada(nombre, nInodo);

	// Devuelve un puntero al fichero
	return fich;
}

void EDirectorio::borrarFichero(char* nombre) {
	// Obtiene el número de inodo del fichero
	long nInodo = idFichero(nombre);

	// Lee el inodo
	InodoESF inodo;
	inodo = sf->leerInodo(nInodo);

	// Abre el fichero
	Fichero* fich;
	switch (inodo.tipo) {
		case NORMAL:
			fich = new EFichero(sf, nInodo, inodo);
			break;
		case DIRECTORIO:
			fich = new EDirectorio(sf, nInodo, inodo);
			break;
		default:
			throw "tipo de fichero desconocido";
	}

	// Borra el contenido del fichero
	fich->borrar();

	delete fich;

	// Libera el inodo
	sf->liberarInodo(nInodo);

	// Libera la entrada del directorio
	borrarEntrada(nombre);
}

long EDirectorio::infoFichero(InfoFichero& info) {
	EEntradaDirectorio entrada;
	InodoESF inodo;

	// Lee una entrada ocupada del directorio
	while(leer((char*)&entrada, sizeof(entrada))==sizeof(entrada))
		if (entrada.nInodo>=0) {
			// Obtiene la información sobre la entrada
			strcpy(info.nombre, entrada.nombre);
			inodo = sf->leerInodo(entrada.nInodo);
			info.tipo = inodo.tipo;
			info.tamano = inodo.tamano;

			// Indica que encontró una entrada ocuapada
			return 1;
		}

	// Indica que ya no quedan más ficheros en este directorio
	return 0;
}
