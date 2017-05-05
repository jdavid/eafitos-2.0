//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/sf/eafitsf.cpp
//	Implementación del sistema de ficheros EAFIT. La parte que
//	entiende el formato del disco.
// --------------------------------------------------------------------

#include <string.h>

#include <eafitsf.h>
#include <bloque.h>
#include <sf.h>


void EafitSF::leerMapaDatos() {
	for (long i=0; i<superbloque.numBloquesMD; i++)
		dispositivo->leer(mapaDatos+tamBloque*i, superbloque.primerBloqueMD+i);
}

void EafitSF::escribirMapaDatos() {
	if (mapaDatos)
		for (long i=0; i<superbloque.numBloquesMD; i++)
			dispositivo->escribir(mapaDatos+tamBloque*i, superbloque.primerBloqueMD+i);
}

void EafitSF::leerMapaInodos() {
	for (long i=0; i<superbloque.numBloquesMI; i++)
		dispositivo->leer(mapaInodos+tamBloque*i, superbloque.primerBloqueMI+i);
}

void EafitSF::escribirMapaInodos() {
	if (mapaDatos)
		for (long i=0; i<superbloque.numBloquesMI; i++)
			dispositivo->escribir(mapaInodos+tamBloque*i, superbloque.primerBloqueMI+i);
}



EafitSF::EafitSF(ControladorBloque* d) : SF(d) {
	mapaDatos = mapaInodos = NULL;

	// Lee el superbloque
	char* buffer = new char[tamBloque];
	dispositivo->leer(buffer, 0);
	memcpy(&superbloque, buffer, sizeof(superbloque));
        delete buffer;

	// Comprueba que sea válido
	if (superbloque.id != EAFITSF_ID)
		throw "este disco no tiene uns sitema de ficheros Eafit";

	// Carga los mapas de bits
	mapaDatos = new char[superbloque.numBloquesMD * tamBloque];
        leerMapaDatos();

	mapaInodos = new char[superbloque.numBloquesMI * tamBloque];
	leerMapaInodos();
}

EafitSF::EafitSF(ControladorBloque* d, long numInodos) : SF(d) {
	mapaDatos = mapaInodos = NULL;

	char* buffer = new char[tamBloque];

	// Debe garantizarse un minimo de inodos, para que el mapa de bits
	// ocupe al menos un bloque. 
	if (numInodos < tamBloque)
        	numInodos = tamBloque;

        // numero de inodos que tiene cada bloque
	long inodosXBloque = tamBloque/sizeof(InodoESF);

	// Construcción y escritura del superbloque
	SuperbloqueESF sb;
	sb.id = EAFITSF_ID;

	sb.numBloquesMI = numInodos / tamBloque;
	sb.numBloquesI = numInodos / inodosXBloque;
	long resto = numBloques - 1 - sb.numBloquesMI - sb.numBloquesI;
	sb.numBloquesMD = resto / (tamBloque + 1);
	if (resto % (tamBloque + 1))
        	sb.numBloquesMD++;
	sb.numBloquesD = resto - sb.numBloquesMD;

	sb.primerBloqueMD = 1;
	sb.primerBloqueMI = sb.primerBloqueMD + sb.numBloquesMD;
	sb.primerBloqueI = sb.primerBloqueMI + sb.numBloquesMI;
	sb.primerBloqueD = sb.primerBloqueI + sb.numBloquesI;

	memcpy(buffer, &sb, sizeof(sb));
	dispositivo->escribir(buffer, 0);


	// Escritura de los mapas de bits, de bloques de datos y de inodos
	// inicializados a LIBRE.
	char* mapaBits = new char[tamBloque];
	memset(mapaBits,_LIBRE,tamBloque);
	long i;
	for (i=sb.primerBloqueMD; i<sb.primerBloqueMI; i++)
		dispositivo->escribir(mapaBits,i);
	for (i=sb.primerBloqueMI; i<sb.primerBloqueI; i++)
		dispositivo->escribir(mapaBits,i);

	// Crea el directorio raiz
	InodoESF raiz;
	raiz.tipo = DIRECTORIO;
	raiz.tamano = 0;
	memset(&raiz.bloquesDirectos, _LIBRE, NBDIRECTOS*sizeof(long));
	raiz.bloquesIndirectos = _LIBRE;

        memcpy(buffer, &raiz, sizeof(raiz));
	dispositivo->escribir(buffer, sb.primerBloqueI);

	mapaBits[0] = _OCUPADO;
	dispositivo->escribir(mapaBits,sb.primerBloqueMI);

	delete mapaBits;
        delete buffer;
}

EafitSF::~EafitSF() {
	escribirMapaInodos();
	escribirMapaDatos();
}


Fichero* EafitSF::abrir(long idFichero) {
	Fichero* fich = NULL;
	InodoESF inodo = leerInodo(idFichero);
	switch (inodo.tipo) {
		case NORMAL:
			fich = new EFichero(this, idFichero, inodo);
			break;
		case DIRECTORIO:
			fich = new EDirectorio(this, idFichero, inodo);
			break;
	}
	return fich;
}


long EafitSF::reservarInodo() {
	unsigned long i;
	for (i=0; i<superbloque.numBloquesI*(tamBloque/sizeof(InodoESF)); i++)
		if (mapaInodos[i] == _LIBRE) {
			mapaInodos[i] = _OCUPADO;
			return i;
		}
	throw "no quedan inodos libres";
}

void EafitSF::liberarInodo(long nInodo) {
	if (mapaInodos[nInodo] == _LIBRE)
		throw "ese inodo ya está libre";
	mapaInodos[nInodo] = _LIBRE;
}

long EafitSF::reservarBloque() {
	for (long i=0; i<superbloque.numBloquesD; i++)
		if (mapaDatos[i] == _LIBRE) {
			mapaDatos[i] = _OCUPADO;
			return i;
		}
	throw "no quedan bloques libres";
}

void EafitSF::liberarBloque(long nBloque) {
	if (mapaDatos[nBloque] == _LIBRE)
		throw "ese inodo ya está libre";
	mapaDatos[nBloque] = _LIBRE;
}


InodoESF& EafitSF::leerInodo(long num) {
	static InodoESF inodo;

	// Calcula el número de inodos por bloque
	long inodosXBloque = tamBloque/sizeof(InodoESF);

	// Lee el bloque que contiene al inodo
	char* buffer = new char[tamBloque];
	dispositivo->leer(buffer,superbloque.primerBloqueI+num/inodosXBloque);

	// Extrae el inodo del bloque
	memcpy(&inodo, buffer+(num%inodosXBloque)*sizeof(InodoESF),
		sizeof(InodoESF));

	// Libera la memoria utilizada
	delete buffer;

	// Devuelve el inodo
	return inodo;
}

void EafitSF::escribirInodo(InodoESF& inodo, long num) {
	// Calcula el número de inodos por bloque
	long inodosXBloque = tamBloque/sizeof(InodoESF);

	// Lee el bloque que contiene al inodo
	char* buffer = new char[tamBloque];
	dispositivo->leer(buffer, superbloque.primerBloqueI +
					num/inodosXBloque);

	// Inserta el inodo en el bloque
	memcpy(buffer + (num%inodosXBloque)*sizeof(InodoESF), &inodo,
		sizeof(InodoESF));

	// Escribe el bloque en el disco
	dispositivo->escribir(buffer, superbloque.primerBloqueI +
					num/inodosXBloque);

	// Libera la memoria utilizada
	delete buffer;
}

long EafitSF::leerBloque(long nBloque, char* buffer) {
	return dispositivo->leer(buffer, superbloque.primerBloqueD + nBloque);
}

long EafitSF::escribirBloque(long nBloque, char* buffer) {
	return dispositivo->escribir(buffer, superbloque.primerBloqueD + nBloque);
} 
