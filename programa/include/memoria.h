//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/memoria.h
//	Definici�n del manejador de memoria del n�cleo.
// --------------------------------------------------------------------

#ifndef _MANEJADOR_MEMORIA
#define _MANEJADOR_MEMORIA


class ManejadorMemoria {
public:
	void iniciar() {}
	void terminar() {}

	// M�todos para reservar y liberar memoria
	long asignar(long cantidad);
	long reservar(long segmento);
	long liberar(long segmento);

	// M�todos para copiar datos de memoria de n�cleo a memoria de usuario
	//   y viceversa
	void aUsuario(long segmento, long dtno, char* fte, long nBytes);
	void deUsuario(long segmento, long fte, char* dtno, long nBytes);
};

#endif
