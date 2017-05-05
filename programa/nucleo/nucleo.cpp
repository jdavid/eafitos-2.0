//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/nucleo.cpp
//	Implementación del núcleo del SO.
// --------------------------------------------------------------------

#include <nucleo.h>
#include <mv.h>


ManejadorMemoria Nucleo::manejadorMemoria;
ManejadorProcesos Nucleo::manejadorProcesos;
DispositivosCaracter Nucleo::dispositivosCaracter;
DispositivosBloque Nucleo::dispositivosBloque;
SFV Nucleo::sfv;

// "Arranca" el núcleo
void Nucleo::iniciar() {
	manejadorMemoria.iniciar();
	manejadorProcesos.iniciar();
	dispositivosCaracter.iniciar();
	dispositivosBloque.iniciar();
	sfv.iniciar();
}

// Termina el núcleo
void Nucleo::terminar() {
	sfv.terminar();
	dispositivosBloque.terminar();
	dispositivosCaracter.terminar();
	manejadorProcesos.terminar();
	manejadorMemoria.terminar();
}

// Rutina de servicio de la interrupción de reloj que genera la CPU
void Nucleo::reloj() {
	// Gestionar las colas de controladores
        long r1 = dispositivosCaracter.gestionarColas();
        long r2 = dispositivosBloque.gestionarColas();

	// Llama al planificador del manejador de hilos y procesos
        try {
                manejadorProcesos.planificador();
        }
        catch (...) {
		if (r1 || r2)
			throw NoHayHilosListos();
		else
			throw NoHayHilos();
	}
}

// Interfaz al núcleo para las llamadas al sistema
void Nucleo::llamada() {
	CPU& cpu = MaqVirtual::cpu;
	Contexto& contexto = cpu.obtenerContexto();
	ManejadorMemoria& manejadorMemoria = Nucleo::manejadorMemoria;

	// Obtención del hilo que se está ejecutando ahora, para saber
	//   después si hubo cambio de contexto.
	long hilo = manejadorProcesos.actual();

	// Obtener el proceso en ejecución.
	Proceso* proceso = manejadorProcesos.procesoEjecucion();

	// Declaración de variables que luego usaremos como parametros en
	//   las llamadas al sistema.
	long param1, param2, param3, res;
	char cadena[100];
	cadena[99] = '\0';

	// Lee el primer valor de los parametros: el servicio que se solicita
	long servicio = cpu.desapilar();

	// En funcion del servicio extraigo el resto de parametros y realizo
	//   la llamada que corresponda.
	try {
		switch (servicio) {
			case CREAR_HILO:
				param1 = cpu.desapilar();
				res = manejadorProcesos.crearHilo(param1);
				break;
			case TERMINAR_HILO:
				param1 = cpu.desapilar();
				manejadorProcesos.terminarHilo(param1);
				break;
			case TERMINAR:
				manejadorProcesos.terminarHilo();
				break;
			case CAMBIAR_UNIDAD:
				param1 = cpu.desapilar();
				proceso->cambiarUnidad(param1);
				break;
			case CAMBIAR_DIRECTORIO:
				param1 = cpu.desapilar();
				manejadorMemoria.deUsuario(contexto.codigo,
							param1, cadena, 99);
				proceso->cambiarDirectorio(cadena);
				break;
			case CREAR_FICHERO:
				param1 = cpu.desapilar();
				manejadorMemoria.deUsuario(contexto.codigo,
							param1, cadena, 99);
				param2 = cpu.desapilar();
				res = proceso->crear(cadena, param2);
				break;
			case ABRIR_FICHERO:
				param1 = cpu.desapilar();
				manejadorMemoria.deUsuario(contexto.codigo,
							param1, cadena, 99);
				res = proceso->abrir(cadena);
				break;
			case CERRAR_FICHERO:
				param1 = cpu.desapilar();
				proceso->cerrar(param1);
				break;
			case BORRAR_FICHERO:
				param1 = cpu.desapilar();
				manejadorMemoria.deUsuario(contexto.codigo,
							param1, cadena, 99);
				proceso->borrar(cadena);
				break;
			case LEER_FICHERO:
				param1 = cpu.desapilar();
				param2 = cpu.desapilar();
				manejadorMemoria.deUsuario(contexto.codigo,
							param2, cadena, 99);
				param3 = cpu.desapilar();
				res = proceso->leer(param1, cadena, param3);
				break;
			case ESCRIBIR_FICHERO:
				param1 = cpu.desapilar();
				param2 = cpu.desapilar();
				manejadorMemoria.deUsuario(contexto.codigo,
							param2, cadena, 99);
				param3 = cpu.desapilar();
				res = proceso->escribir(param1, cadena, param3);
				break;
			case SALTAR_FICHERO:
				param1 = cpu.desapilar();
				param2 = cpu.desapilar();
				param3 = cpu.desapilar();
				res = proceso->saltar(param1, param2, param3);
				break;
			case EJECUTAR:
				param1 = cpu.desapilar();
				manejadorMemoria.deUsuario(contexto.codigo,
							param1, cadena, 99);
				proceso->ejecutar(cadena);
				break;
			case LEER_ENTRADA_ESTANDAR:
				res = proceso->leerCaracter();
				break;
			case ESCRIBIR_SALIDA_ESTANDAR:
				param1 = cpu.desapilar();
				proceso->imprimirCaracter((char)param1);
				break;
			default:
				res = -1;
		}
	}
	catch(...) {
		res = -1;
	}

	// Si no ha habido cambio de contexto, devuelve el resultado al hilo
	//   en el registro general 0
	try {
		if (hilo == manejadorProcesos.actual())
			contexto.registros[0] = res;
	}
	catch(...) {}
}
