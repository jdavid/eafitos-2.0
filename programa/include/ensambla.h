//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/ensambla.h
//	Definición del ensamlador.
// --------------------------------------------------------------------

#ifndef _ENSAMBLADOR
#define _ENSAMBLADOR


// Posibles tokens
enum Tokens {DATOS, CODIGO, FINAL_FICHERO, NUEVA_LINEA, ID, COMA,
	REGISTRO, LITERAL_NUMERICO, CADENA,
	I_, I_R, I_RR, I_RRR, I_I, I_RI};

/* Explicación del significado de cada tipo de instrucción
 I_	: sin operandos: especiales (NOP, SER_SIS)
 I_R 	: un registro: de gestión de la pila
 I_RR	: dos registros: operaciones de ALU unarias y acceso a memoria
 I_RRR	: tres registros: operaciones aritmético/lógicas
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

// Aqui guardamos el lexema de un identificador y su dirección de memoria.
class Id {
public:
	char lexema[20];
	long direccion;
};


class Ensamblador {
	// Ficheros fuente y objeto
	long ffte;
	long fobj;

	// Número de línea en el código fuente que se está analizando
	long linea;

	// Variables que guardan información sobre el token actual
	char lexema[100];
	long token;
	long valor;

	// Función que implementa el analizador léxico. Lee el siguiente token.
	long siguienteToken();

	long pasada;	// Indica si estamos en la 1ª o en la 2ª pasada
	long direccion;	// Dirección actual dentro del código objeto

	// Información que se recogera en la 1ª pasada y se usara en la 2ª
	Id ids[100];
	long nIds;
	long dirInicio;

	// Funciones para gestionar la cola de identificadores.
	void nuevoId();
	long obtenerDir(char* nombre);

	// Funciones que implementan el analizador sintáctico, semántico y
	//   generador de código.
	// Una función para cada símbolo no terminal.
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
