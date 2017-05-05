//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	nucleo/hilo.cpp
//	Implementación de un hilo.
// --------------------------------------------------------------------

#include <hilo.h>
#include <mv.h>
#include <nucleo.h>


Hilo::Hilo() {
	estado = LIBRE;
}

// Devuelve el proceos asociado
Proceso* Hilo::obtenerProceso() {
	return proceso;
}

// Devuelve la petición de E/S
Peticion& Hilo::obtenerPeticion() {
	return peticion;
}

// Métodos para poner la petición
void Hilo::ponerPeticion(Peticion::Tipo codigo) {
	peticion.codigo = codigo;
}

void Hilo::ponerPeticion(Peticion::Tipo codigo, long dato) {
	peticion.codigo = codigo;
	peticion.dato = dato;
}

void Hilo::ponerPeticion(Peticion::Tipo codigo, char* datos) {
	peticion.codigo = codigo;
	peticion.datos = datos;
}

void Hilo::ponerPeticion(Peticion::Tipo codigo, long dato, char* datos) {
	peticion.codigo = codigo;
	peticion.dato = dato;
	peticion.datos = datos;
}

// ¿Está libre esta entrada en la tabla de hilos?
char Hilo::libre() {
	if (estado==LIBRE)
		return 1;
	return 0;
}

// ¿Está este hilo en estado LISTO?
char Hilo::listo() {
	if (estado==LISTO)
		return 1;
	return 0;
}


// Métodos para las transiciones entre estados.
//   Crea un hilo a partir de otro (para la llamada al sistema CREAR_HILO)
void Hilo::iniciar(Proceso* proc, long direccion) {
	// Inicializa el contexto del nuevo hilo
	contexto = MaqVirtual::cpu.obtenerContexto();
	contexto.pc = direccion;

	// Reserva la memoria necesaria
	Nucleo::manejadorMemoria.reservar(contexto.codigo);
	contexto.pila = Nucleo::manejadorMemoria.asignar(128);

	// Asocia e inicia el proceso
	proceso = proc;
	proceso->iniciar();

	// Pasa el hilo a LISTO
	estado = LISTO;
}

//   Crea un hilo nuevo para un proceso nuevo (ejecución de programas)
void Hilo::iniciar(Proceso* proc, Contexto& ctx) {
	// Inicializa el contexto del nuevo hilo
	contexto = ctx;

	// Asocia el proceso
	proceso = proc;

	// Pasa el hilo a LISTO
	estado = LISTO;
}

//   Pasa de LISTO a EJECUCION
void Hilo::ejecutar() {
	MaqVirtual::cpu.ponerContexto(contexto);
	estado = EJECUCION;
}

//   Pasa de EJECUCION a LISTO
void Hilo::dormir() {
	contexto = MaqVirtual::cpu.obtenerContexto();
	estado = LISTO;
}

//   Pasa de EJECUCION a SUSPENDIDO
void Hilo::suspender() {
	contexto = MaqVirtual::cpu.obtenerContexto();
	estado = SUSPENDIDO;
}

//   Pasa de SUSPENDIDO a LISTO
void Hilo::reactivar(long resultado) {
	contexto.registros[0] = resultado;
	estado = LISTO;
}

//   Termina un hilo, da igual en que estado se enecuentre
void Hilo::terminar() {
	// Libera la memoria asociada
	Nucleo::manejadorMemoria.liberar(contexto.codigo);
	Nucleo::manejadorMemoria.liberar(contexto.pila);

	// Termina el proceso
	proceso->terminar();

	// Pone el hilo en LIBRE
	estado = LIBRE;
}
