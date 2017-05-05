//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/sf/sfv.cpp
//	Implementación del sistema de ficheros virtual.
// --------------------------------------------------------------------

#include <string.h>

#include <sf.h>
#include <nucleo.h>
#include <eafitsf.h>


long SFV::buscar(long unidad, long nInodo) {
	// Recorre la tabla de ficheros
	for (long i=0; i<MAX_FICHEROS_ABTOS; i++)
		// Si el fichero existe...
		if (ficheros[i])
			// ...si el fichero es el buscado devuelve su id.
			if (ficheros[i]->es(sf[unidad], nInodo))
				return i;

	return -1;
}

// Inicializa el sistema de ficheros
void SFV::iniciar() {
	long i;

	// Inicializa la tabla de ficheros
	for (i=0; i<MAX_FICHEROS_ABTOS; i++)
		ficheros[i] = NULL;

	// Inicializa la tabla de sitemas de ficheros
	for (i=0; i<MAX_NUM_SF; i++) {
		sf[i] = NULL;
		try {
			ControladorBloque* cB;
			cB = Nucleo::dispositivosBloque.obtenerControlador(i);
			iniSF(cB, i);
		}
		catch(...) {}
	}
}

void SFV::terminar() {
	long i;

	// Limpia la tabla de ficheros
	for (i=0; i<MAX_FICHEROS_ABTOS; i++)
		if (ficheros[i])
			while (cerrar(i));

	// Limpia la tabla de sistemas de ficheros
	for (i=0; i<MAX_NUM_SF; i++)
		if (sf[i])
			delete sf[i];
}


// Inicializa un sistema de ficheros
void SFV::iniSF(ControladorBloque* cB, long n) {
	if (cB==NULL)
		throw "ese dispositivo no existe";

	try {
		sf[n] = new EafitSF(cB);
	}
	catch(...) {
		// Aquí añadir soporte para otros sistemas de ficheros
		throw "sistema de ficheros desconocido";
	}

	// Abrir el directorio raiz
	for (long i=0; i<MAX_FICHEROS_ABTOS; i++)
		if (ficheros[i]==NULL) {
			ficheros[i] = sf[n]->abrir(0);
			sf[n]->ponerDirRaiz(i);
			return;
		}

	delete sf[n];
	throw "demasiados ficheros abiertos";
}

long SFV::activo(long id) {
	if (id<0 || id>=MAX_NUM_SF)
		throw "número de unidad fuera de rango";

	if (sf[id]==NULL)
		return 0;

	return 1;
}


long SFV::obtenerTamano(long id) {
	if (id<0 || id>=MAX_FICHEROS_ABTOS || ficheros[id]==NULL)
		throw "ese fichero no está abierto";

	return ficheros[id]->obtenerTamano();
}


long SFV::crear(long unidad, long pwd, char *nombre, long tipo) {
	Fichero* fich;
	char *token1, *token2;
	long id, nInodo, temporal=0;

	// Determina si es una ruta absoluta o relativa, obtiene el directorio
	//   inicial donde empezará la busqueda (fich)
	if (nombre[0]=='/') {
		id = sf[unidad]->obtenerDirRaiz();
		fich = ficheros[id];
	} else
		fich = ficheros[pwd];

	token1 = strtok(nombre, "/");
	token2 = strtok(NULL, "/");
	while (token2!=NULL) {
		nInodo = fich->idFichero(token1);
		if (temporal)
 			delete fich;

                // Comprueba si el fichero ya está abierto
		id = buscar(unidad, nInodo);
		if (id!=-1) {
			temporal = 0;
			fich = ficheros[id];
		} else {
			// Lo abre temporalmente
			temporal = 1;
			fich = sf[unidad]->abrir(nInodo);
		}

		// Siguiente subdirectorio
		token1 = token2;
		token2 = strtok(NULL, "/");
	}

	// Busca una entrada libre, si no hay genera un error
	for (id=0; id<MAX_FICHEROS_ABTOS && ficheros[id]!=NULL; id++);
	if (id>=MAX_FICHEROS_ABTOS)
		throw "demasiados ficheros abiertos";

	// Crea el fichero
	ficheros[id] = fich->crearFichero(token1, tipo);

	if (temporal)
		delete fich;

	return id;
}

long SFV::abrir(long unidad, long pwd, char *nombre) {
	Fichero* fich;
	char* cad;
	long id, nInodo, temporal=0;

	// Determina si es una ruta absoluta o relativa, obtiene el directorio
	//   inicial donde empezará la busqueda (fich)
	if (nombre[0]=='/') {
		id = sf[unidad]->obtenerDirRaiz();
		fich = ficheros[id];
	} else
		fich = ficheros[pwd];

	cad = strtok(nombre, "/");
	while (cad!=NULL) {
		nInodo = fich->idFichero(cad);
		if (temporal)
			delete fich;

		// Comprueba si el fichero ya está abierto
		id = buscar(unidad, nInodo);
		if (id!=-1) {
			temporal = 0;
			fich = ficheros[id];
		} else {
			// Lo abre temporalmente
			temporal = 1;
			fich = sf[unidad]->abrir(nInodo);
		}

		// Siguiente subdirectorio
		cad = strtok(NULL, "/");
        }

	// Abre el fichero
	if (id==-1) {
		// Busca una entrada libre, si no hay genera error
		for (id=0; id<MAX_FICHEROS_ABTOS && ficheros[id]!=NULL; id++);
		if (id>=MAX_FICHEROS_ABTOS)
			throw "demasiados ficheros abiertos";

		ficheros[id] = fich;
	} else
		// Si ya estaba abierto solo incrementa su uso
		ficheros[id]->abrir();

	return id;
}

// Incrementa el uso de un fichero que ya está abierto
long SFV::abrir(long id) {
	// Comprueba que el identificador se válido
	if (id<0 || id>=MAX_FICHEROS_ABTOS || ficheros[id]==NULL)
		throw "ese fichero no está abierto";

	return ficheros[id]->abrir();
}

long SFV::cerrar(long id) {
	// Comprueba que el identificador se válido
	if (id<0 || id>=MAX_FICHEROS_ABTOS || ficheros[id]==NULL)
		throw "ese fichero no está abierto";

	// Cierra el fichero
	long uso = ficheros[id]->cerrar();
	if (uso==0) {
		delete ficheros[id];
		ficheros[id] = NULL;
	}
	return uso;
}

void SFV::borrar(long unidad, long pwd, char* nombre) {
	Fichero* fich;
	char *token1, *token2;
	long id, nInodo, temporal=0;

	// Determina si es una ruta absoluta o relativa, obtiene el directorio
	//   inicial donde empezará la busqueda (fich)
	if (nombre[0]=='/') {
		id = sf[unidad]->obtenerDirRaiz();
		fich = ficheros[id];
	} else
		fich = ficheros[pwd];

	token1 = strtok(nombre, "/");
	token2 = strtok(NULL, "/");
	while (token2!=NULL) {
		nInodo = fich->idFichero(token1);
		if (temporal)
			delete fich;

		// Comprueba si el fichero ya está abierto
		id = buscar(unidad, nInodo);
		if (id!=-1) {
			temporal = 0;
			fich = ficheros[id];
		} else {
			// Lo abre temporalmente
			temporal = 1;
			fich = sf[unidad]->abrir(nInodo);
		}

		// Siguiente subdirectorio
		token1 = token2;
		token2 = strtok(NULL, "/");
	}

	// Borra el fichero
	fich->borrarFichero(token1);

	if (temporal)
		delete fich;
}

long SFV::leer(long id, void *destino, long numBytes) {
	if (id<0 || id>=MAX_FICHEROS_ABTOS || ficheros[id]==NULL)
		throw "ese fichero no está abierto";

	return ficheros[id]->leer((char*)destino, numBytes);
}

long SFV::escribir(long id, void *fuente, long numBytes) {
	if (id<0 || id>=MAX_FICHEROS_ABTOS || ficheros[id]==NULL)
		throw "ese fichero no está abierto";

	return ficheros[id]->escribir((char*)fuente, numBytes);
}

long SFV::saltar(long id, long posicion, long desde) {
	if (id<0 || id>=MAX_FICHEROS_ABTOS || ficheros[id]==NULL)
		throw "ese fichero no está abierto";

	return ficheros[id]->saltar(posicion, desde);
}


long SFV::infoFichero(long id, InfoFichero& info) {
	if (id<0 || id>=MAX_FICHEROS_ABTOS || ficheros[id]==NULL)
		throw "ese fichero no está abierto";

	return ficheros[id]->infoFichero(info);
}
