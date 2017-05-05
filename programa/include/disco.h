//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/disco.h
//	Definici�n del controlador del disco.
// --------------------------------------------------------------------

#ifndef _DISCO
#define _DISCO


#include <stdio.h>

// Tama�o de bloque + numero de bloques
#define CABECERA sizeof(long)+sizeof(long)


class Disco {
	FILE* disco;

	long tamBloque;
	long numBloques;

	// A partir del n�mero del disco devuelve su nombre completo
	//   "\EAFITOS\DISCO_n" o "$HOME/EAFITOS/DISCO_n"
	char* obtenerNombre(char id);
public:
	// Constructor para inicializar un disco
	Disco(char id);	
	// Constructor para formatear un disco
	Disco(char id, long tBloque, long nBloques);
        ~Disco();

	long obtenerTamBloque();
	long obtenerNumBloques();

	// M�todos de acceso al disco, leen/ecriben un bloque.
	long leer(char *buffer, long bloque);
	long escribir(char *buffer, long bloque);
};


#endif
