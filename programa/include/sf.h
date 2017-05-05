//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/sf.h
//	Definición del sistema de ficheros.
// --------------------------------------------------------------------

#ifndef _SISTEMA_DE_FICHEROS
#define _SISTEMA_DE_FICHEROS


#include <cbloque.h>

// Constantes para el Sistema de Ficheros Virtual
#define MAX_FICHEROS_ABTOS 100
#define MAX_NUM_SF 10

// Tipos de ficheros
#define NORMAL 0
#define DIRECTORIO 1

// Constantes para el método saltar de la clase Fichero
#define PRINCIPIO 0
#define ACTUAL 1


// Clase utilizada como abstracción de la información de directorio
class InfoFichero {
public:
	char nombre[200];
	long tipo;
	long tamano;
};


// Clase base para los sistemas de ficheros reales
class SF {
protected:
	// Controlador de tipo bloque asociado a este sistema de ficheros
	ControladorBloque* dispositivo;

	long tamBloque;
	long numBloques;

	// Guarda el identificador del directorio raiz en el sistema de
	//   ficheros virtual.
	long dirRaiz;
public:
	SF(ControladorBloque* d) {
		dispositivo = d;
		tamBloque = dispositivo->obtenerTamBloque();
		numBloques = dispositivo->obtenerNumBloques();
		dirRaiz = -1;
	}
	virtual ~SF() {}

	long obtenerTamBloque()		{ return tamBloque; }
	long obtenerNumBloques()	{ return numBloques; }

	long obtenerDirRaiz()		{ return dirRaiz; }
	void ponerDirRaiz(long dir)	{ dirRaiz = dir; }

	// Métodos que todos los sistemas de ficheros deben implementar.
	virtual class Fichero* abrir(long idFichero) = 0;
};


// Clase base para los ficheros de los sistemas de ficheros reales
class Fichero {
protected:
	long uso;	// Número de elementos que estan usando este fichero.
	long posicion;  // Apunta al próximo caracter a leer.

	long tamBloque;	// Tamaño del bloque del sistema de ficheros
	char* buffer;	// Buffer para leer y escribir bloques.
public:
	Fichero(SF* sf) {
		uso = 1;
		posicion = 0;

		tamBloque = sf->obtenerTamBloque();
		buffer = new char[tamBloque];
	}

	virtual ~Fichero() {
		if (buffer)
			delete buffer;
	}

	// Métodos generales que todos los ficheros deben implementar
	virtual long es(SF* sf, long idFichero) = 0;
	virtual long obtenerTamano() = 0;
	virtual long finalFichero() = 0;

	virtual long abrir() = 0;
	virtual long cerrar() = 0;
	virtual void borrar() = 0;
	virtual long leer(char* dtno, long nDatos) = 0;
	virtual long escribir(char* fte, long nDatos) = 0;
	virtual long saltar(long pos, long desde) = 0;

	// Métodos que solo los directorios deben implementar
	//   Devuelve el identificador del fichero (nº de inodo, por ejemplo)
	virtual long idFichero(char* nombre)
		{ throw "función no implementada"; }

	//   Devuelve cierto o falso según el fichero exista o no
	virtual long existeFichero(char* nombre)
		{ throw "función no implementada"; }

	//   Crea el fichero indicado
	virtual Fichero* crearFichero(char* nombre, char tipo)
		{ throw "función no implementada"; }

	//   Borra el fichero indicado
	virtual void borrarFichero(char* nombre)
		{ throw "función no implementada"; }

	//   Devuelve información de un fichero del directorio
	virtual long infoFichero(InfoFichero& info)
		{ throw "función no implementada"; }
};


// Sistema de ficheros virtual
class SFV {
	// Tabla de sistemas de ficheros reales
	SF* sf[MAX_NUM_SF];

	// Tabla de ficheros abiertos en el sistema
	Fichero* ficheros[MAX_FICHEROS_ABTOS];

	// Método que busca el fichero indicado en la tabla de ficheros
	long buscar(long unidad, long nInodo);
public:
	void iniciar();
	void terminar();

	// Inicializa un sistema de ficheros
	void iniSF(ControladorBloque* cB, long n);

	// Devuelve cierto o falso según si el sistema de ficheros está o no
	//   inicializado
	long activo(long id);

	long obtenerTamano(long id);

	// Métodos para las llamadas al sistema
	long crear(long unidad, long pwd, char *nombre, long tipo);
	long abrir(long unidad, long pwd, char *nombre);
	long abrir(long id);
	long cerrar(long id);
	void borrar(long unidad, long pwd, char* nombre);
	long leer(long id, void *destino, long numBytes);
	long escribir(long id, void *fuente, long numBytes);
	long saltar(long id, long posicion, long desde);

	// Devuelve información sobre el fichero indicado
	long infoFichero(long id, InfoFichero& info);
};


#endif
