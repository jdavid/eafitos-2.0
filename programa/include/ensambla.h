//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/ensambla.h
//	Definici�n del ensamlador.
// --------------------------------------------------------------------

#ifndef _ENSAMBLADOR
#define _ENSAMBLADOR


// Posibles tokens
enum Tokens {DATOS, CODIGO, FINAL_FICHERO, NUEVA_LINEA, ID, COMA,
	REGISTRO, LITERAL_NUMERICO, CADENA,
	I_, I_R, I_RR, I_RRR, I_I, I_RI};

/* Explicaci�n del significado de cada tipo de instrucci�n
 I_	: sin operandos: especiales (NOP, SER_SIS)
 I_R 	: un registro: de gesti�n de la pila
 I_RR	: dos registros: operaciones de ALU unarias y acceso a memoria
 I_RRR	: tres registros: operaciones aritm�tico/l�gicas
 I_I	: un dato inmediato: salto incondicional
 I_RI	: un registro y un dato inmediato: acceso a memoria y salto condicional
*/


// Define cada palabra clave
class PalabraClave {
public:
	char* lexema;
	long token;
	long valor;
};

// Aqui guardamos el lexema de un identificador y su direcci�n de memoria.
class Id {
public:
	char lexema[20];
	long direccion;
};


class Ensamblador {
	// Ficheros fuente y objeto
	long ffte;
	long fobj;

	// N�mero de l�nea en el c�digo fuente que se est� analizando
	long linea;

	// Variables que guardan informaci�n sobre el token actual
	char lexema[100];
	long token;
	long valor;

	// Funci�n que implementa el analizador l�xico. Lee el siguiente token.
	long siguienteToken();

	long pasada;	// Indica si estamos en la 1� o en la 2� pasada
	long direccion;	// Direcci�n actual dentro del c�digo objeto

	// Informaci�n que se recogera en la 1� pasada y se usara en la 2�
	Id ids[100];
	long nIds;
	long dirInicio;

	// Funciones para gestionar la cola de identificadores.
	void nuevoId();
	long obtenerDir(char* nombre);

	// Funciones que implementan el analizador sint�ctico, sem�ntico y
	//   generador de c�digo.
	// Una funci�n para cada s�mbolo no terminal.
	void programa();
	void datos();
	void lineaDatos();
	void constante();
	void codigo();
	void lineaCodigo();
	void instruccion();
public:
	void analizar(char* fuente, char* objeto, long unidad, long directorio);
};


#endif
