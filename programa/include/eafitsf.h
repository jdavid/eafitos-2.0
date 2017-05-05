//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/eafitsf.h
//	Definición del sistema de ficheros EAFIT.
// --------------------------------------------------------------------


#ifndef _SF_EAFIT
#define _SF_EAFIT

#include <sf.h>


// Posibles estados de las entradas de los mapas de datos e inodos.
#define _LIBRE 0
#define _OCUPADO 1

// Numero de bloques directos por inodo.
#define NBDIRECTOS 7

// Maxima longitud del nombre que puede tener un directorio.
#define LONG_NOMBRE 20

// Número mágico
#define EAFITSF_ID 1500

// El superbloque se encuentra en el primer bloque del disco (bloque 0).
// Se pueden añadir más campos al final, siempre y cuando el superbloque no
// sobrepase el tamaño de un bloque.
class SuperbloqueESF {
public:
	long id;	// Número mágico

	long primerBloqueMD;	// MD: Mapa de bits de los bloques de Datos
	long numBloquesMD;
	long primerBloqueMI;	// MI: Mapa de bits de los Inodos
	long numBloquesMI;
	long primerBloqueI;	// I: Inodos
	long numBloquesI;
	long primerBloqueD;	// D: bloques de Datos
	long numBloquesD;
};



// Cada fichero viene representado por un inodo.
// Los inodos del sistema de ficheros Eafit ocupan 64 bytes.
// Los primeros contienen información varia sobre el fichero
// A continuación se reserva un espacio para uso futuro.
// Los ultimos 32 bytes almacenan las direcciones de los bloques de datos.
class InodoESF {
public:
	long tamano;
	char tipo;

	char reservado[27];

	long bloquesDirectos[NBDIRECTOS];
	long bloquesIndirectos;
};


// Clase que conoce el formato del Sistema de Ficheros Eafit.
class EafitSF : public SF {
	SuperbloqueESF superbloque;

	char* mapaDatos;
	char* mapaInodos;

	// Lee y escribe los mapas de bits del sistema de ficheros
	void leerMapaDatos();
	void escribirMapaDatos();
	void leerMapaInodos();
	void escribirMapaInodos();
public:
	// Constructores:
	//   carga un sistema de ficheros ya creado
	EafitSF(ControladorBloque* d);
	//   da formato a un disco
	EafitSF(ControladorBloque* d, long numInodos);
	~EafitSF();

	// Abre el fichero con el identificador dado
	Fichero* abrir(long idFichero = 0);

	// Reserva y libera inodos y bloques
	long reservarInodo();
	void liberarInodo(long nInodo);
	long reservarBloque();
	void liberarBloque(long nBloque);

	// Lee y escribe inodos del disco
	InodoESF& leerInodo(long num);
	void escribirInodo(InodoESF& inodo, long n);

	// Lee y escribe bloques del disco
	long leerBloque(long nBloque, char* buffer);
	long escribirBloque(long nBloque, char* buffer); 
};


// Fichero del Sistema de Ficheros Eafit.
class EFichero : public Fichero {
protected:
	EafitSF* sf;	// Sistema de Ficheros Eafit que contiene este fichero.
	long nInodo;	// Numero de Inodo.

	InodoESF inodo;		 // Guarda en memoria una copia del inodo.
	long* bloquesIndirectos; // Indices a los bloques indirectos.

	// Devuelve el núm. de bloque actual, si es final de fichero genera
	//   un error
	long obtenerNBloque();
	// Devuelve el núm. de bloque actual, si es final de fichero reserva
	//   un byte.
	long reservarBloque();
public:
	EFichero(EafitSF* sf, long nInodo, InodoESF& inodo);
	~EFichero();

	// Cierto o falso según este fichero sea o no el buscado
	long es(SF* sf, long idFichero);

	// Devuelve el tamaño del fichero
	long obtenerTamano();

	// Cierto o falso según estemos o no en el final del bloque
	long finalFichero();

	// Operaciones varias
	long abrir();
	long cerrar();
	virtual void borrar();
	long leer(char* dtno, long nDatos);
	long escribir(char* fte, long nDatos);
	long saltar(long pos, long desde);
};



// Una entrada de directorio del Sistema de Ficheros Eafit.
class EEntradaDirectorio {
public:
	long nInodo;
	char nombre[LONG_NOMBRE];
	char reservado[8];
};


// Un fichero de tipo directorio del Sistema de Ficheros Eafit.
class EDirectorio : public EFichero {
	// Crea y borra entradas de deirectorio
	void crearEntrada(char* nombre, long nInodo);
	void borrarEntrada(char* nombre);
public:
	EDirectorio(EafitSF* sf, long nInodo, InodoESF& inodo) :
		EFichero(sf, nInodo, inodo) {}

	void borrar();

	long idFichero(char* nombre);
	long existeFichero(char* nombre);
	Fichero* crearFichero(char* nombre, char tipo);
	void borrarFichero(char* nombre);
	long infoFichero(InfoFichero& info);
};



#endif
