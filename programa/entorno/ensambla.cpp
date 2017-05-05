//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	entorno/ensambla.cpp
//	Implementación el Ensamblador.
// --------------------------------------------------------------------

#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ensambla.h>
#include <nucleo.h>
#include <cpu.h>


PalabraClave palabrasClave[] = {
	{"DATOS", DATOS, 0}, {"CODIGO", CODIGO, 0},
	{"SUMAR", I_RRR, SUMAR}, {"RESTAR", I_RRR, RESTAR},
	{"AND", I_RRR, SUMAR}, {"OR", I_RRR, RESTAR},
	{"COPIAR", I_RR, COPIAR}, {"NOT", I_RR, RESTAR},
	{"CARGAR32", I_RR, CARGAR32}, {"GUARDAR32", I_RR, GUARDAR32},
	{"CARGAR8", I_RR, CARGAR8}, {"GUARDAR8", I_RR, GUARDAR8},
	{"CARGAR_I", I_RI, CARGAR_I}, {"GUARDAR_I", I_RI, GUARDAR_I},
	{"APILAR", I_R, APILAR}, {"DESAPILAR", I_R, DESAPILAR},
	{"SALTAR", I_I, SALTAR}, {"SALTAR0", I_RI, SALTAR0},
	{"SALTARP", I_RI, SALTARP}, {"SALTARN", I_RI, SALTARN},
	{"NOP", I_, NOP}, {"SER_SIS", I_, SER_SIS}
};

#define N_PALABRAS_CLAVE 22


long Ensamblador::siguienteToken() {
	SFV& sfv = Nucleo::sfv;

	while (1) {
		long i;
		char c;
		long fin = sfv.leer(ffte, &c, 1);

		if (!fin) {
			token = FINAL_FICHERO;
			return token;
		}
		if (c==';') {
			while (1) {
				fin = sfv.leer(ffte, &c, 1);
				if (c=='\n' || !fin)
					break;
			}
			if (fin)
				sfv.saltar(ffte, -1, ACTUAL);
			continue;
		}
		if (c=='\n') {
			linea++;
			while (1) {
				fin = sfv.leer(ffte, &c, 1);
				if (!fin)
					break;
				if (isspace(c)) {
					if (c=='\n')
						linea++;
					continue;
				}
				if (c==';') {
					while (1) {
						fin = sfv.leer(ffte, &c, 1);
						if (c=='\n' || !fin)
							break;
					}
					if (fin)
						sfv.saltar(ffte, -1, ACTUAL);
					continue;
				}
				sfv.saltar(ffte, -1, ACTUAL);
				break;
			}
			token = NUEVA_LINEA;
			return token;
		}
		if (isspace(c))
			continue;
		if (c==',') {
			token = COMA;
			return token;
		}
		if (c=='@') {
			for (i=0; i<2; i++) {
				fin = sfv.leer(ffte, &c, 1);
				if (!fin)
					break;
				if (isdigit(c))
					lexema[i]=c;
				else {
					sfv.saltar(ffte, -1, ACTUAL);
					break;
				}
			}
			if (i==0)
				throw "falta número de registro";
			else {
				lexema[i]='\0';
				valor = atol(lexema);
				if (valor>=16)
					throw "número de registro demasiado grande";
				token = REGISTRO;
				return token;
			}
		}
		if (c=='#') {
			for (i=0; i<12; i++) {
				fin = sfv.leer(ffte, &c, 1);
				if (!fin)
					break;
				if (isdigit(c))
					lexema[i]=c;
				else {
					sfv.saltar(ffte, -1, ACTUAL);
					break;
				}
			}
			if (i==0)
				throw "falta dato inmediato";
			else {
				lexema[i]='\0';
				valor = atol(lexema);
				token = LITERAL_NUMERICO;
				return token;
			}
		}
		if (c=='"') {
			for (i=0; i<100; i++) {
				fin = sfv.leer(ffte, &c, 1);
				if (!fin)
					throw "fin de fichero inesperado";
				if (c=='"')
					break;
				if (c=='\\') {
					fin = sfv.leer(ffte, &c, 1);
					if (!fin)
						throw "fin de fichero inesperado";
					if (c=='n')
						c = '\n';
					if (c=='t')
						c = '\t';
				}
				lexema[i] = c;
			}
			if (i>=100)
				throw "cadena demasiado larga (máximo 100 caracteres)";
			lexema[i] = '\0';
			token = CADENA;
			return token;
		}
		if (isalpha(c)) {
			lexema[0] = toupper(c);
			for (i=1; i<20; i++) {
				fin = sfv.leer(ffte, &c, 1);
				if (!fin || (!isalnum(c) && c!='_')) {
					lexema[i]='\0';
					if (fin)
						sfv.saltar(ffte, -1, ACTUAL);
					for (i=0; i<N_PALABRAS_CLAVE; i++)
						if (strcmp(lexema,palabrasClave[i].lexema)==0) {
							token = palabrasClave[i].token;
							valor = palabrasClave[i].valor;
							return token;
						}
					token = ID;
					return token;
				}
				lexema[i] = toupper(c);
			}
		}

		throw "caracter desconocido";
	}
}



void Ensamblador::nuevoId() {
	if (nIds>=100)
		throw "demasiados identificadores (máximo 100)";

	for (long i=0; i<nIds; i++)
		if (!strcmp(lexema,ids[i].lexema))
			throw "declaración múltiple de un identificador";

	strcpy(ids[nIds].lexema, lexema); 
	ids[nIds].direccion = direccion;
	nIds++;
}

long Ensamblador::obtenerDir(char* nombre) {
	for (long i=0; i<nIds; i++)
		if (strcmp(nombre, ids[i].lexema)==0)
			return ids[i].direccion;

	throw "identificador no declarado";
}


void Ensamblador::programa() {
	SFV& sfv = Nucleo::sfv;

	// Escribe la cabecera del fichero ejecutable, formato 99 (2ª pasada)
	if (pasada==1) {
		long cod;
		cod = 2323;
		sfv.escribir(fobj, &cod, 4);
		sfv.escribir(fobj, &dirInicio, 4);
		cod = 128;
		sfv.escribir(fobj, &cod, 4);
	}

	// Lee el primer token del programa
	siguienteToken();

	// Si son comentarios o líneas vacias, lee el siguiente token
	if (token==NUEVA_LINEA)
		siguienteToken();

	// Si hay sección de datos la procesa
	if (token==DATOS) {
		siguienteToken();
		if (token!=NUEVA_LINEA)
			throw "nueva linea esperada";
		datos();
	}

	// Comprueba que la sección de código empice por "CODIGO NUEVA_LINEA"
	if (token!=CODIGO)
		throw "CODIGO esperado";

	siguienteToken();
	if (token!=NUEVA_LINEA)
		throw "nueva linea esperada";

	// Guarda la dirección de inicio del código para la cabecera
	if (pasada==0)
		dirInicio = direccion;

	// Procesa el código
	siguienteToken();
	codigo();

	// Añade la instrucción implicita para finalizar el hilo (2ª pasada)
	if (pasada==0)
		direccion+=9;
	else if (pasada==1) {
		char byte = CARGAR_I;
		sfv.escribir(fobj, &byte, 1);
		byte = 0;
		sfv.escribir(fobj, &byte, 1);
		long dato = 3;
		sfv.escribir(fobj, &dato, 4);
		byte = APILAR;
		sfv.escribir(fobj, &byte, 1);
		byte = 0;
		sfv.escribir(fobj, &byte, 1);
		byte = SER_SIS;
		sfv.escribir(fobj, &byte, 1);
	}
}

void Ensamblador::datos() {
	siguienteToken();

	if (token!=CODIGO) {
		lineaDatos();

		siguienteToken();
		if (token!=NUEVA_LINEA)
			throw "nueva linea esperada";

		datos();
	}
}

void Ensamblador::lineaDatos() {
	if (token!=ID)
		throw "identificador esperado";

	// Añadir la variable a la lista
	if (pasada==0)
		nuevoId();

	constante();
}

void Ensamblador::constante() {
	SFV& sfv = Nucleo::sfv;

	siguienteToken();

	if (token==LITERAL_NUMERICO) {
		if (pasada==0)
			direccion+=4;
		else if (pasada==1)
			sfv.escribir(fobj, &valor, 4);
	} else if (token==CADENA) {
		if (pasada==0)
			direccion += strlen(lexema)+1;
		else if (pasada==1)
			for (unsigned long i=0; i<=strlen(lexema); i++)
				sfv.escribir(fobj, &lexema[i], 1);
	} else
		throw "constante esperada";
}

void Ensamblador::codigo() {
	lineaCodigo();

	if (token!=FINAL_FICHERO) {
		if (token!=NUEVA_LINEA)
			throw "nueva linea esperada";

		siguienteToken();

		if (token!=FINAL_FICHERO)
			codigo();
	}
}

void Ensamblador::lineaCodigo() {
	if (token==ID) {
		// Añadir la etiqueta a la lista
		if (pasada==0)
			nuevoId();
		
		siguienteToken();
		if (token==NUEVA_LINEA || token==FINAL_FICHERO)
			return;
	}

	instruccion();
	siguienteToken();
}

void Ensamblador::instruccion() {
	SFV& sfv = Nucleo::sfv;

	switch(token) {
		case I_:
			// Escribimos el código de instrucción
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Incrementamos la dirección de memoria
			if (pasada==0)
				direccion++;
			break;
		case I_R:
			// Escribimos el código de instrucción
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos el registro y lo escribimos
			siguienteToken();
			if (token!=REGISTRO)
				throw "registro esperado";
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Incrementamos la dirección de memoria
			if (pasada==0)
				direccion+=2;
			break;
		case I_RR:
			// Escribimos el código de instrucción
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos el primer registro y lo escribimos
			siguienteToken();
			if (token!=REGISTRO)
				throw "registro esperado";
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos la coma
			siguienteToken();
			if (token!=COMA)
				throw "coma esperada";
			// Leemos el segundo registro y lo escribimos
			siguienteToken();
			if (token!=REGISTRO)
				throw "registro esperado";
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Incrementamos la dirección de memoria
			if (pasada==0)
				direccion+=3;
			break;
		case I_RRR:
			// Escribimos el código de instrucción
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos el primer registro y lo escribimos
			siguienteToken();
			if (token!=REGISTRO)
				throw "registro esperado";
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos la coma
			siguienteToken();
			if (token!=COMA)
				throw "coma esperada";
			// Leemos el segundo registro y lo escribimos
			siguienteToken();
			if (token!=REGISTRO)
				throw "registro esperado";
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos la coma
			siguienteToken();
			if (token!=COMA)
				throw "coma esperada";
			// Leemos el tercer registro y lo escribimos
			siguienteToken();
			if (token!=REGISTRO)
				throw "registro esperado";
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Incrementamos la dirección de memoria
			if (pasada==0)
				direccion+=4;
			break;
		case I_I:
			// Escribimos el código de instrucción
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos el dato inmediato y lo escribimos
			siguienteToken();
			if (pasada==1) {
				if (token==ID)
					valor = obtenerDir(lexema);
				else if (token!=LITERAL_NUMERICO)
					throw "identificador o número esperado";
				sfv.escribir(fobj, &valor, 4);
			}
			// Incrementamos la dirección de memoria
			if (pasada==0)
				direccion+=5;
			break;
		case I_RI:
			// Escribimos el código de instrucción
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos el registro y lo escribimos
			siguienteToken();
			if (token!=REGISTRO)
				throw "registro esperado";
			if (pasada==1)
				sfv.escribir(fobj, &valor, 1);
			// Leemos la coma
			siguienteToken();
			if (token!=COMA)
				throw "coma esperada";
			// Leemos el dato inmediato y lo escribimos
			siguienteToken();
			if (pasada==1) {
				if (token==ID)
					valor = obtenerDir(lexema);
				else if (token!=LITERAL_NUMERICO)
					throw "identificador o número esperado";
				sfv.escribir(fobj, &valor, 4);
			}
			// Incrementamos la dirección de memoria
			if (pasada==0)
				direccion+=6;
			break;
		case FINAL_FICHERO:
			break;
		default:
			throw "instrucción desconocida";
	}
}


void Ensamblador::analizar(char* fte, char* obj, long unidad, long dir) {
	SFV& sfv = Nucleo::sfv;

	// Inicialización de algunas variables
	ffte = fobj = -1;
	linea = 1;
	pasada = 0;
	direccion = 0;
	nIds = 0;

	try {
		// Abrimos el fichero fuente
		ffte = sfv.abrir(unidad, dir, fte);
		if (ffte==-1)
			throw "no se pudo abrir el fichero fuente";

		// Primera pasada. Análisis léxico y sintáctico, obtención de
		// información para la segunda pasada
		programa();

		// Creamos el fichero objeto
		fobj = sfv.crear(unidad, dir, obj, 0);
		if (fobj==-1)
			throw "no se pudo crear el fichero objeto";

		// Segunda pasada. Análisis semántico y generación de código
		sfv.saltar(ffte, 0, PRINCIPIO);
		linea = 1;
		pasada = 1;
		programa();

		// Cerramos los ficheros
		sfv.cerrar(ffte);
		sfv.cerrar(fobj);
	}
	catch(char* msg) {
		// Cerramos los ficheros abiertos y borramos el código generado
		if (ffte>=0) {
			sfv.cerrar(ffte);
			if (fobj>=0) {
				sfv.cerrar(fobj);
				sfv.borrar(unidad, dir, obj);
			}
		}

		printw("Error en la linea %d, %s\n", linea, msg);
		refresh();
		return;
	}

	addstr("Código generado con éxito\n");
	refresh();
}
