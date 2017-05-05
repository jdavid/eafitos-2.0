//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	include/bloque.h
//	Definición de la lista de controladores de tipo bloque.
// --------------------------------------------------------------------

#ifndef _DISPOSITIVOS_BLOQUE
#define _DISPOSITIVOS_BLOQUE


#include <cbloque.h>


#define NUM_DISPOS_BLOQUE 10


// Tabla de dispositivos de tipo bloque
class DispositivosBloque {
	// Tabla de controladores de tipo bloque
	ControladorBloque* dispositivos[NUM_DISPOS_BLOQUE];
public:
	void iniciar();
	void terminar() {}

	// Inicializa el controlador con el identificador especificado
	void iniDisco(long id);

	// Devuelve el controlador con el identificador especificado
	ControladorBloque* obtenerControlador(long id);

	// Método encargado de gestionar las colas de hilos asociadas a
	//   los controladores de tipo bloque cada vez que se produce
	//   una interrupción de reloj
	long gestionarColas();
};


#endif
