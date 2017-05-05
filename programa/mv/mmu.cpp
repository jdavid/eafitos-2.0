//	Copyright (C) 1998-1999 Juan David Ib��ez Palomar
//	Este c�digo es libre. Est� protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para m�s detalles.
//
// --------------------------------------------------------------------
//	mv/mmu.cpp
//	Implementaci�n de la unidad de gesti�n de memoria.
// --------------------------------------------------------------------

#include <mmu.h>
#include <mv.h>


// Constructor, pone el segmento y todas las p�ginas en libre.
Segmento::Segmento() {
	uso = 0;

	// Inicializar las p�ginas a libre
	for (long i=0; i<PAGS_POR_SEGMENTO; i++)
		paginas[i] = -1;
}


long Segmento::libre() {
	return !uso;
}

// Reserva memoria
long Segmento::reservar(long nPaginas) {
	if (uso)
		throw "ese segmento ya est� ocupado";

	// Reserva las p�ginas necesarias
	long paginaLogica=0;
	for (; nPaginas>0; nPaginas--) {
		long paginaFisica = MaqVirtual::mmu.reservarPagina();
		paginas[paginaLogica] = paginaFisica;
		paginaLogica++;
	}
	uso = 1;

	return uso;
}

// Incrementa el uso del segmento
long Segmento::reservar() {
	if (!uso)
		throw "ese segmento est� libre";
	uso++;
	return uso;
}

// Liberar el segmento
long Segmento::liberar() {
	if (!uso)
		throw "ese segmento ya est� libre";

	// Si ya no utiliza nadie este segmento, liberar las p�ginas que tenia
	//   ocupadas
	uso--;
	if (uso==0)
		for (long i=0; i<PAGS_POR_SEGMENTO; i++)
			if (paginas[i]>=0) {
				MaqVirtual::mmu.liberarPagina(paginas[i]);
				paginas[i] = -1;
			}

	return uso;
}

// Obtiene la p�gina f�sica asociada a la pagina l�gica dada
long Segmento::paginaFisica(long paginaLogica) {
	if (paginaLogica<0 || paginaLogica>=PAGS_POR_SEGMENTO)
		throw "n�mero de p�gina fuera de rango";

	return paginas[paginaLogica];
}



MMU::MMU() {
	// Poner todas las p�ginas f�sicas como libres
	for (long i=0; i<NUM_PAGINAS; i++)
		paginas[i] = 0;

	nPaginasLibres = NUM_PAGINAS;
}

// Traduce una direcci�n f�sica a una direcci�n l�gica
long MMU::logicaAFisica (long segmento, long dirLogica) {
	long paginaLogica, paginaFisica, desplazamiento;

	// Calcula la p�gina f�sica
	paginaLogica = dirLogica/TAM_PAGINA;
	paginaFisica = segmentos[segmento].paginaFisica(paginaLogica);

	// Fallo de p�gina
	if (paginaFisica<0)
		throw "fallo de p�gina";

	// Calcula el desplazamiento
	desplazamiento = dirLogica%TAM_PAGINA;

	// Calcula y devuelve la direcci�n f�sica
	return (paginaFisica*TAM_PAGINA) + desplazamiento;
}

// Reserva una p�gina f�sica
long MMU::reservarPagina() {
	// Busca y reserva una p�gina f�sica, devuelve su identificador
	for (long i=0; i<NUM_PAGINAS; i++)
		if (paginas[i]==0) {
			paginas[i] = 1;
			nPaginasLibres--;
			return i;
		}

	throw "todas las p�ginas est�n ocupadas";
}

// Libera la p�gina f�sica indicada
void MMU::liberarPagina(long pagina) {
	if (paginas[pagina]==0)
		throw "esa p�gina ya est� libre";

	paginas[pagina] = 0;
	nPaginasLibres++;
}

// Reserva un segmento y la cantidad de memoria solicitada, devuelve el
//   selector del segmento
long MMU::asignar(long cantidad) {
	// Calcula el n�mero de p�ginas necesarias
	long nPaginas = cantidad/TAM_PAGINA;
	if (cantidad%TAM_PAGINA)
		nPaginas++;

	// Comprueba que haya suficiente memoria
	if (nPaginas>nPaginasLibres)
		throw "no hay suficente memoria";

	// Busca un segmento libre
	for (long i=0; i<NUM_SEGMENTOS; i++)
		if (segmentos[i].libre()) {
			segmentos[i].reservar(nPaginas);
			return i;
		}

	throw "todos los segmentos est�n ocupados";
}

// Incrementa el uso de un segmento ya reservado
long MMU::reservar(long segmento) {
	return segmentos[segmento].reservar();
}

// Libera un segmento
long MMU::liberar(long segmento) {
	return segmentos[segmento].liberar();
}
