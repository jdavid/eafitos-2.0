//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	mv/mmu.cpp
//	Implementación de la unidad de gestión de memoria.
// --------------------------------------------------------------------

#include <mmu.h>
#include <mv.h>


// Constructor, pone el segmento y todas las páginas en libre.
Segmento::Segmento() {
	uso = 0;

	// Inicializar las páginas a libre
	for (long i=0; i<PAGS_POR_SEGMENTO; i++)
		paginas[i] = -1;
}


long Segmento::libre() {
	return !uso;
}

// Reserva memoria
long Segmento::reservar(long nPaginas) {
	if (uso)
		throw "ese segmento ya está ocupado";

	// Reserva las páginas necesarias
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
		throw "ese segmento está libre";
	uso++;
	return uso;
}

// Liberar el segmento
long Segmento::liberar() {
	if (!uso)
		throw "ese segmento ya está libre";

	// Si ya no utiliza nadie este segmento, liberar las páginas que tenia
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

// Obtiene la página física asociada a la pagina lógica dada
long Segmento::paginaFisica(long paginaLogica) {
	if (paginaLogica<0 || paginaLogica>=PAGS_POR_SEGMENTO)
		throw "número de página fuera de rango";

	return paginas[paginaLogica];
}



MMU::MMU() {
	// Poner todas las páginas físicas como libres
	for (long i=0; i<NUM_PAGINAS; i++)
		paginas[i] = 0;

	nPaginasLibres = NUM_PAGINAS;
}

// Traduce una dirección física a una dirección lógica
long MMU::logicaAFisica (long segmento, long dirLogica) {
	long paginaLogica, paginaFisica, desplazamiento;

	// Calcula la página física
	paginaLogica = dirLogica/TAM_PAGINA;
	paginaFisica = segmentos[segmento].paginaFisica(paginaLogica);

	// Fallo de página
	if (paginaFisica<0)
		throw "fallo de página";

	// Calcula el desplazamiento
	desplazamiento = dirLogica%TAM_PAGINA;

	// Calcula y devuelve la dirección física
	return (paginaFisica*TAM_PAGINA) + desplazamiento;
}

// Reserva una página física
long MMU::reservarPagina() {
	// Busca y reserva una página física, devuelve su identificador
	for (long i=0; i<NUM_PAGINAS; i++)
		if (paginas[i]==0) {
			paginas[i] = 1;
			nPaginasLibres--;
			return i;
		}

	throw "todas las páginas están ocupadas";
}

// Libera la página física indicada
void MMU::liberarPagina(long pagina) {
	if (paginas[pagina]==0)
		throw "esa página ya está libre";

	paginas[pagina] = 0;
	nPaginasLibres++;
}

// Reserva un segmento y la cantidad de memoria solicitada, devuelve el
//   selector del segmento
long MMU::asignar(long cantidad) {
	// Calcula el número de páginas necesarias
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

	throw "todos los segmentos están ocupados";
}

// Incrementa el uso de un segmento ya reservado
long MMU::reservar(long segmento) {
	return segmentos[segmento].reservar();
}

// Libera un segmento
long MMU::liberar(long segmento) {
	return segmentos[segmento].liberar();
}
