//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/proceso.cpp
//	Implementación de un proceso.
// --------------------------------------------------------------------

#include <string.h>

#include <proceso.h>
#include <nucleo.h>


Proceso::Proceso() {
	uso = 0;
}

// ¿Está libre el proceso?
long Proceso::libre() {
	return !uso;
}


// Inicia el proceso o incrementa su uso si el proceso ya existe
long Proceso::iniciar(long u, long d) {
	if (uso==0) {
		// Inicializa la entrada/salida estandar
		abrir("--teclado--");
		abrir("--pantalla--");

		// Inicializa la unidad y el directorio actuales
		unidad = u;
		directorio = d;
		Nucleo::sfv.abrir(directorio);
	}
	uso++;

	return uso;
}

// Termina el proceso
long Proceso::terminar() {
	// Reduce el uso del proceso
	uso--;

	// Si el uso es cero lo termina
	if (uso==0) {
		// Cierra todos los ficheros abiertos de este proceso
		for (long i=0; i<N_F_LOCALES; i++) {
			if (!ficherosLocales[i].libre())
				ficherosLocales[i].cerrar();
		}

		// Cierra el directorio actual del proceso
		Nucleo::sfv.cerrar(directorio);
	}

	return uso;
}

// Cambia la unidad actual
void Proceso::cambiarUnidad(long num) {
	// Si la nueva unidad existe cambia;
	if (Nucleo::sfv.activo(num)) {
		// Cierra el directorio actual
		Nucleo::sfv.cerrar(directorio);

		// Cambia a la nueva unidad
		unidad = num;

		// Abre el directorio raíz de la nueva unidad como el nuevo
		//   directorio actual 
		directorio = Nucleo::sfv.abrir(unidad, 0, "/");
	}

	// si no, genera un error
	throw "esa unidad no existe";
}

// Cambia el directorio actual
void Proceso::cambiarDirectorio(char* nombre) {
	// Abre el nuevo directorio
	long dir = Nucleo::sfv.abrir(unidad, directorio, nombre);

	// Cierra el directorio actual
	Nucleo::sfv.cerrar(directorio);

	// Actualiza el directorio actual
	directorio = dir;
}

// Crea un nuevo fichero
long Proceso::crear(char* nombre, long tipo) {
	// Busca una entrada libre en la tabla de ficheros locales,
	for (long fL=0; fL<N_F_LOCALES; fL++)
		if (ficherosLocales[fL].libre()) {
			// Crea el fichero
			ficherosLocales[fL].crear(unidad, directorio,
							nombre, tipo);

			// Devuelve el identificador local del fichero
			return fL;
		}

	//  si no la encuentra genera un error
	throw "demasiados ficheros abiertos";
}

// Abre un fichero
long Proceso::abrir(char* nombre) {
	// Busca una entrada libre en la tabla de ficheros locales,
	for (long fL=0; fL<N_F_LOCALES; fL++)
		if (ficherosLocales[fL].libre()) {
			// Abre el fichero
			ficherosLocales[fL].abrir(unidad, directorio, nombre);

			// Devuelve el identificador local del fichero
			return fL;
		}

	//  si no la encuentra genera un error
	throw "demasiados ficheros abiertos";
}

// Cierra un fichero
void Proceso::cerrar(long id) {
	// Si el identificador está fuera de rango genera un error
	if (id<0 || id>=N_F_LOCALES)
		throw "número de fichero fuera de rango";

	// Cierra el fichero
	ficherosLocales[id].cerrar();
}

// Lee de un fichero
long Proceso::leer(long id, void* destino, long nBytes) {
	if (id<0 || id>=N_F_LOCALES)
		throw "número de fichero fuera de rango";

	return ficherosLocales[id].leer(destino, nBytes);
}

// Escribe en un fichero
long Proceso::escribir(long id, void* fuente, long nBytes) {
	if (id<0 || id>=N_F_LOCALES)
		throw "número de fichero fuera de rango";

	return ficherosLocales[id].escribir(fuente, nBytes);
}

// Cambia la porsición dentro de un fichero
long Proceso::saltar(long id, long posicion, long desde) {
	if (id<0 || id>=N_F_LOCALES)
		throw "número de fichero fuera de rango";

	return ficherosLocales[id].saltar(posicion, desde);
}

// Lee un carácter de la entrada estándar
long Proceso::leerCaracter() {
	char c;
	ficherosLocales[0].leer(&c,1);
	return (long)c;
}

// Imprime un carácter en la salida estándar
void Proceso::imprimirCaracter(char c) {
	ficherosLocales[1].escribir(&c, 1);
}

// Borra un fichero
void Proceso::borrar(char* nombre) {
	Nucleo::sfv.borrar(unidad, directorio, nombre);
}

// Ejecuta un programa
void Proceso::ejecutar(char* nombre) {
	Nucleo::manejadorProcesos.ejecutar(unidad, directorio, nombre);
}
