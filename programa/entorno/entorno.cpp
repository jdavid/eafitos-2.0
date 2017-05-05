//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	entorno.cc
//	Este fichero tan solo contiene la función main.
// --------------------------------------------------------------------


#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include <entorno.h>
#include <cdisco.h>
#include <eafitsf.h>
#include <mv.h>
#include <nucleo.h>


InterpreteComandos Entorno::interpreteComandos;
Ensamblador Entorno::ensamblador;


void Entorno::crearDiscoNuevo() {
	clear();
	addstr("\nCREAR UN DISCO NUEVO\n\n");
	addstr("Elige el disco a crear (0-4)\n");
	addstr("Pulsa x para volver atras\n");
	refresh();

	char car;
	do {
		car = getch();
	} while ((car<'0' || car>'4') && car!='x' && car!='X');

	if (car=='x' || car=='X')
		return;

	try {
		MaqVirtual::crearDisco(car-'0', 1024, 1024);
		Nucleo::dispositivosBloque.iniDisco(car-'0');
	}
	catch (char* cad) {
		printw("Error, %s\n", cad);
		refresh();
	}
}

void Entorno::formatearDisco() {
	clear();
	addstr("\nFORMATEAR UN DISCO\n\n");
	addstr("Elige el disco a formatear (0-4)\n");
	addstr("Pulsa x para volver atras\n");
	refresh();

	char car;
	do {
		car = getch();
	} while ((car<'0' || car>'4') && car!='x' && car!='X');

	if (car=='x' || car=='X')
		return;

	try {
		ControladorBloque *cB = Nucleo::dispositivosBloque.obtenerControlador(car-'0');
		EafitSF sf(cB, 0);
		Nucleo::sfv.iniSF(cB, car-'0');
	}
	catch (char* cad) {
		printw("Error, %s\n", cad);
		refresh();
	}
}


void Entorno::iniciar() {
	// Inicializamos Curses
	initscr();
	noecho();
	cbreak();
	idlok(stdscr, TRUE);
	scrollok(stdscr, TRUE);


	char car;
	do {
		clear();
		addstr("\n****  SISTEMA OPERATIVO EAFITOS  ****\n\n");
		addstr("1- Interprete de Comandos\n");
		addstr("2- Crear un nuevo disco\n");
		addstr("3- Dar formato a un disco ya creado");
		addstr(" (Sistema de ficheros Eafit)\n");
		addstr("0- Salir\n");
		refresh();

		do {
			car = getch();
		} while (car<'0' || car>'3');

		switch (car) {
			case '0': break;
			case '1': interpreteComandos.iniciar();
				  break;
			case '2': crearDiscoNuevo();
				  break;
			case '3': formatearDisco();
		}
	} while (car!='0');

	addstr("Gracias por usar EafitOS\n");
	refresh();

	// Finalizamos Curses
	endwin();
}
