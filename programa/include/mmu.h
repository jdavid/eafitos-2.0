//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	include/mmu.h
//	Definici�n de la unidad de gesti�n de memoria
// --------------------------------------------------------------------

#ifndef _MMU
#define _MMU


#include <memfis.h>

// Relacionado con la gesti�n de la memoria
#define NUM_SEGMENTOS 20
#define PAGS_POR_SEGMENTO 5
#define TAM_PAGINA 1024
#define NUM_PAGINAS TAM_MEMORIA/TAM_PAGINA


// Clase que define un segmento como un conjunto de p�ginas.
// Un valor de -1 en paginas[x] significa que la p�gina x est� libre.
class Segmento {
	long uso;
	long paginas[PAGS_POR_SEGMENTO];
public:
	Segmento();

	long libre();
	long reservar(long nPaginas);
	long reservar();
	long liberar();

	long paginaFisica(long paginaLogica);
};


// Unidad de gesti�n de memoria.
// Obtiene direcciones f�sicas a partir de direcciones l�gicas.
// Reserva y libera memoria.
class MMU {
	Segmento segmentos[NUM_SEGMENTOS];

	// Para cada pagina indicamos si esta ocupada (>0) o no (=0)
	char paginas[NUM_PAGINAS];
	long nPaginasLibres;
public:
	MMU();

	// Traduce una direccion logica a una direccion fisica
	long logicaAFisica(long segmento, long dirLogica);

	// Reserva/libera una p�gina
	long reservarPagina();
	void liberarPagina(long pagina);

	// Reserva/libera memoria
	long asignar(long memoria);
	long reservar(long segmento);
	long liberar(long segmento);
};


#endif
