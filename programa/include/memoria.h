//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/memoria.h
//	Definición del manejador de memoria del núcleo.
// --------------------------------------------------------------------

#ifndef _MANEJADOR_MEMORIA
#define _MANEJADOR_MEMORIA


class ManejadorMemoria {
public:
	void iniciar() {}
	void terminar() {}

	// Métodos para reservar y liberar memoria
	long asignar(long cantidad);
	long reservar(long segmento);
	long liberar(long segmento);

	// Métodos para copiar datos de memoria de núcleo a memoria de usuario
	//   y viceversa
	void aUsuario(long segmento, long dtno, char* fte, long nBytes);
	void deUsuario(long segmento, long fte, char* dtno, long nBytes);
};

#endif
