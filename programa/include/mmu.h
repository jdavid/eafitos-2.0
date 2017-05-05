//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/mmu.h
//	Definición de la unidad de gestión de memoria
// --------------------------------------------------------------------

#ifndef _MMU
#define _MMU


#include <memfis.h>

// Relacionado con la gestión de la memoria
#define NUM_SEGMENTOS 20
#define PAGS_POR_SEGMENTO 5
#define TAM_PAGINA 1024
#define NUM_PAGINAS TAM_MEMORIA/TAM_PAGINA


// Clase que define un segmento como un conjunto de páginas.
// Un valor de -1 en paginas[x] significa que la página x está libre.
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


// Unidad de gestión de memoria.
// Obtiene direcciones físicas a partir de direcciones lógicas.
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

	// Reserva/libera una página
	long reservarPagina();
	void liberarPagina(long pagina);

	// Reserva/libera memoria
	long asignar(long memoria);
	long reservar(long segmento);
	long liberar(long segmento);
};


#endif
