//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	mv/cpu.cpp
//	Implementación de la unidad central de proceso.
// --------------------------------------------------------------------

#include <cpu.h>
#include <mv.h>
#include <nucleo.h>


// Escribir un byte en memoria
void CPU::escribirByte(long segmento, long direccion, char dato) {
	long dirFisica = MaqVirtual::mmu.logicaAFisica(segmento, direccion);
	MaqVirtual::memoria.escribirByte(dato, dirFisica);
}

// Escribir una palabra en memoria
void CPU::escribirPalabra(long segmento, long direccion, long dato) {
	long dirFisica = MaqVirtual::mmu.logicaAFisica(segmento, direccion);
	MaqVirtual::memoria.escribirPalabra(dato, dirFisica);
}

// Leer un byte de memoria
char CPU::leerByte(long segmento, long direccion) {
	long dirFisica = MaqVirtual::mmu.logicaAFisica(segmento, direccion);
	return MaqVirtual::memoria.leerByte(dirFisica);
}

// Leer una palabra de memoria
long CPU::leerPalabra(long segmento, long direccion) {
	long dirFisica = MaqVirtual::mmu.logicaAFisica(segmento, direccion);
	return MaqVirtual::memoria.leerPalabra(dirFisica);
}

// Leer un operando de tamaño de un byte 
char CPU::leerOpByte() {
	return leerByte(contexto.codigo, contexto.pc++);
}

// Leer un operando de tamaño de una palabra
long CPU::leerOpPalabra() {
	long dato = leerByte(contexto.codigo, contexto.pc);
	contexto.pc += 4;
	return dato;
}

// Apilar un dato en la pila
void CPU::apilar(long dato) {
	escribirPalabra(contexto.pila, contexto.sp, dato);
	contexto.sp+=4;
}

// Desapilar un dato de la pila
long CPU::desapilar() {
	contexto.sp-=4;
	return leerPalabra(contexto.pila, contexto.sp);
}

// Ejecutar una instrucción
void CPU::ejecutarPaso() {
	// Simula una interrupción de reloj. Le cede el control al núcleo.
	Nucleo::reloj();

	// Adquisición de la instrucción
	char instruccion = leerOpByte();

	// Decodificación, adquisición de los operandos y ejecución
	switch (instruccion) {
		long fte1, fte2, destino;
		case SUMAR:
			fte1 = leerOpByte();
			fte2 = leerOpByte();
			destino = leerOpByte();
			contexto.registros[destino] = contexto.registros[fte1]
						+ contexto.registros[fte2];
			break;
		case RESTAR:
			fte1 = leerOpByte();
			fte2 = leerOpByte();
			destino = leerOpByte();
			contexto.registros[destino] = contexto.registros[fte1]
						- contexto.registros[fte2];
			break;
		case AND:
			fte1 = leerOpByte();
			fte2 = leerOpByte();
			destino = leerOpByte();
			contexto.registros[destino] = contexto.registros[fte1]
						& contexto.registros[fte2];
			break;
		case OR:
			fte1 = leerOpByte();
			fte2 = leerOpByte();
			destino = leerOpByte();
			contexto.registros[destino] = contexto.registros[fte1]
						| contexto.registros[fte2];
			break;
		case COPIAR:
			fte1 = leerOpByte();
			destino = leerOpByte();
			contexto.registros[destino] = contexto.registros[fte1];
			break;
		case NOT:
			fte1 = leerOpByte();
			destino = leerOpByte();
			contexto.registros[destino] = ~contexto.registros[fte1];
			break;
		case CARGAR32:
			destino = leerOpByte();
			fte1 = leerOpByte();
			fte1 = leerPalabra(contexto.codigo,
						contexto.registros[fte1]);
			contexto.registros[destino] = fte1;
			break;
		case GUARDAR32:
			fte1 = leerOpByte();
			destino = leerOpByte();
			escribirPalabra(contexto.codigo,
					contexto.registros[destino],
					contexto.registros[fte1]);
			break;
		case CARGAR8:
			destino = leerOpByte();
			fte1 = leerOpByte();
			fte1 = leerByte(contexto.codigo,
					contexto.registros[fte1]);
			contexto.registros[destino] = fte1;
			break;
		case GUARDAR8:
			fte1 = leerOpByte();
			destino = leerOpByte();
			escribirByte(contexto.codigo,
					contexto.registros[destino],
					contexto.registros[fte1]);
			break;
		case CARGAR_I:
			destino = leerOpByte();
			fte1 = leerOpPalabra();
			contexto.registros[destino] = fte1;
			break;
		case GUARDAR_I:
			fte1 = leerOpByte();
			destino = leerOpPalabra();
			escribirPalabra(contexto.codigo, destino,
					contexto.registros[fte1]);
			break;
		case APILAR:
			fte1 = leerOpByte();
			apilar(contexto.registros[fte1]);
			break;
		case DESAPILAR:
			destino = leerOpByte();
			contexto.registros[destino] = desapilar();
			break;
		case SALTAR:
			destino = leerOpPalabra();
			contexto.pc = destino;
			break;
		case SALTAR0:
			fte1 = leerOpByte();
			destino = leerOpPalabra();
			if (contexto.registros[fte1]==0)
				contexto.pc = destino;
			break;
		case SALTARP:
			fte1 = leerOpByte();
			destino = leerOpPalabra();
			if (contexto.registros[fte1]>0)
				contexto.pc = destino;
			break;
		case SALTARN:
			fte1 = leerOpByte();
			destino = leerOpPalabra();
			if (contexto.registros[fte1]<0)
				contexto.pc = destino;
			break;
		case NOP:
			break;
		case SER_SIS:
			Nucleo::llamada();
			break;
		default:
			throw "instrucción desconocida";
	}
}

// Ejecutar instrucciones mientras queden hilos en ejecución
void CPU::ejecutar() {
	while (1) {
		try {
			ejecutarPaso();
		}
		catch(Nucleo::NoHayHilosListos) {}
		catch(Nucleo::NoHayHilos) { return; }
		catch(char* c) {
			Nucleo::manejadorProcesos.terminarHilo();
		}
	}
}
