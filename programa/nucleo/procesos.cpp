//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/procesos.cpp
//	Implementación del manejador de hilos y de procesos.
// --------------------------------------------------------------------

#include <procesos.h>
#include <nucleo.h>
#include <ejec99.h>


void ManejadorProcesos::iniciar() {
	// Registrar formatos de ficheros ejecutables
	ejecutables[0] = new Ejecutable99();
	for (long i=1; i<N_EJECUTABLES; i++)
		ejecutables[i] = 0;
}

void ManejadorProcesos::terminar() {
	// Limpia la tabla de formatos de ficheros ejecutables
	for (long i=0; i<N_EJECUTABLES; i++)
		if (ejecutables[i])
			delete ejecutables[i];
}


Proceso* ManejadorProcesos::procesoEjecucion() {
	return hilos[actual()].obtenerProceso();
}


long ManejadorProcesos::crearHilo(long direccion) {
	// Busca una entrada libre
	long hilo;
	for (hilo=0; hilo<N_HILOS && !hilos[hilo].libre(); hilo++);
	if (hilo==N_HILOS)
		throw "ya no quedan entradas de hilos libres";

	// Inicia el hilo
	Proceso* proceso = hilos[actual()].obtenerProceso();
	hilos[hilo].iniciar(proceso, direccion);

	// Inserta el hilo en la cola
	insertar(hilo);

	return hilo;                       
}

void ManejadorProcesos::terminarHilo(long id) {
	// Si no se especifica el hilo se asume que es el actual
	if (id<0)
		id = actual();

	// Comprueba que el id es válido
	if (id>=N_HILOS)
		throw "número de hilo fuera de rango";

	// Comprueba que el hilo existe
	if (hilos[id].libre())
		throw "ese hilo no existe";

	extraer(id);
	hilos[id].terminar();
}


long ManejadorProcesos::ejecutar(long unidad, long pwd, char* nombre) {
	// Busco una entrada libre de hilo
	long hilo;
	for (hilo=0; hilo<N_HILOS && !hilos[hilo].libre(); hilo++);
	if (hilo==N_HILOS)
		throw "ya no quedan entradas de hilos libres";

	// Busco una entrada libre de proceso
	long proc;
	for (proc=0; proc<N_PROCESOS && !procesos[proc].libre(); proc++);
	if (proc==N_PROCESOS)
		throw "ya no quedan entradas de procesos libres";

	// Abre el fichero.
	long id = Nucleo::sfv.abrir(unidad, pwd, nombre);

	// Prueba a ejecutar el fichero
	Contexto contexto;
	for (long i=0; i<N_EJECUTABLES && ejecutables[i]!=0; i++)
		try {
			ejecutables[i]->ejecutar(id, contexto);

			// Cerrar el fichero
			Nucleo::sfv.cerrar(id);

			// Iniciar el proceso y el hilo
			procesos[proc].iniciar(unidad, pwd);
			hilos[hilo].iniciar(&procesos[proc], contexto);

			// Insertar el hilo en la cola de hilos listos
			insertar(hilo);

			// Regresar con exito
			return hilo;
		}
		catch(...) {}

	// Cerrar el fichero
	Nucleo::sfv.cerrar(id);

	// No se reconoció el fichero.
	throw "fichero no ejecutable";
}


void ManejadorProcesos::planificador() {
	long hilo = actual();

	if (hilos[hilo].listo()) {
		// Si no hay ningún hilo en ejecución pasa el hilo de la
		//   cabeza de la cola de hilos listos a EJECUCIÓN
		hilos[hilo].ejecutar();
		tiempo = TAJADA;
	} else {
		// Si ya hay algún hilo en EJECUCION...
		// Decrementa el contador de tiempo
		tiempo--;

		// Si ya se ha terminado el tiempo del hilo actual
		//   cambia el hilo en ejecución
		if (tiempo==0) {
			// Pasa el hilo en ejecución a LISTO
			hilos[hilo].dormir();
			extraer(hilo);
			insertar(hilo);

			// Pasa el nuevo hilo que es cabeza de la cola de
			//   hilos listos a EJECUCION
			hilos[actual()].ejecutar();

			// Inicializa el contador de tiempo
			tiempo = TAJADA;
		}
	}
}
