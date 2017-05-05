//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	mv/disco.cpp
//	Implementaci�n de un disco virtual.
// --------------------------------------------------------------------

#include <disco.h>

#include <stdio.h>
#include <string.h>
#ifdef _LINUX_
	#include <stdlib.h>
#endif

// A partir del n�mero del disco devuelve su nombre completo
char* Disco::obtenerNombre(char id) {
	static char nombre[50];

	// Si el SO anfitri�n es GNU/Linux, "//EAFITOS/DISCO_n"
	#ifdef _LINUX_
		strcpy(nombre, getenv("HOME"));
		strcat(nombre, "/EAFITOS/DISCO_");
	#endif

	// Si el SO anfitri�n es DOS, "\EAFITOS\DISCO_n"
	#ifdef _DOS_
		strcpy(nombre, "\\EAFITOS\\DISCO_");
	#endif

	nombre[strlen(nombre)] = id;
	nombre[strlen(nombre)+1] = '\0';

	return nombre;
}

// Constructor utilizado para inicializar los discos
Disco::Disco(char id) {
	char* nombre;
	nombre = obtenerNombre(id);

	disco = fopen(nombre, "r+b");
	if (disco == NULL)
		throw "ese disco no existe";

	// Lee el tama�o y el n�mero de bloques del disco
	fread(&tamBloque,sizeof(tamBloque),1,disco);
	fread(&numBloques,sizeof(numBloques),1,disco);
}

// Constructor utilizado para crear los discos
Disco::Disco(char id, long tBloque, long nBloques) {
	tamBloque = tBloque;
	numBloques = nBloques;

	char* nombre;
	nombre = obtenerNombre(id);

	// Crea el fichero
	disco = fopen(nombre, "wb");
	if (disco==NULL)
		throw "no se pudo crear el disco";

	// Escribe el tama�o y el n�mero de bloques del disco
        fwrite(&tamBloque,sizeof(tamBloque),1,disco);
	fwrite(&numBloques,sizeof(numBloques),1,disco);

	// Escribe espacios en el fichero hasta alcanzar el tama�o deseado
	for (long i=0; i<tamBloque*numBloques; i++)
		fprintf(disco, " ");

	freopen(nombre, "r+b", disco);
}

Disco::~Disco() {
	if (disco)
		fclose(disco);
}


long Disco::obtenerTamBloque() {
	return tamBloque;
}

long Disco::obtenerNumBloques() {
	return numBloques;
}

// Leer un bloque
long Disco::leer(char *datos, long bloque) {
	if (bloque<0 || bloque >= numBloques)
		throw "n�mero de bloque no v�lido";

	fseek(disco,CABECERA+bloque*tamBloque,SEEK_SET);
	return fread(datos,tamBloque,1,disco);
}

// Escribir un bloque
long Disco::escribir(char *datos, long bloque) {
	if (bloque<0 || bloque >= numBloques)
		throw "n�mero de bloque no v�lido";

	fseek(disco,CABECERA+bloque*tamBloque,SEEK_SET);
	return fwrite(datos,tamBloque,1,disco);
}
