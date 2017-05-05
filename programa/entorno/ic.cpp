//	Copyright (C) 1998-1999 Juan David Ibáñez Palomar
//	Este código es libre. Está protegido por la licencia GNU GPL.
//	Mira el fichero Copyright para más detalles.
//
// --------------------------------------------------------------------
//	entorno/ic.cpp
//	Implementa el interprete de comandos.
// --------------------------------------------------------------------

#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ic.h>
#include <mv.h>
#include <nucleo.h>
#include <entorno.h>


#ifdef _LINUX_
#define SEPARADOR "/"
#endif
#ifdef _DOS_
#define SEPARADOR "\\"
#endif


void InterpreteComandos::iniciar() {
	SFV& sfv = Nucleo::sfv;

	// Busca el primer disco formateado que exista
	long i;
	for (i=0; i<MAX_NUM_SF; i++)
		if (sfv.activo(i)) {
			// Inicializa la unidad y directorio actuales
			unidad = i;
			directorio = sfv.abrir(i, 0, "/");
			break;
		}

	// Si no existe ningún disco formateado regresa
	if (i>=MAX_NUM_SF)
		return;

	// Limpia la pantalla
	clear();
	long y, x, yMaxima, xMaxima;
	getmaxyx(stdscr, yMaxima, xMaxima);

	// Bucle principal
	while (1) {
		printw("Eafitos (%d): ", unidad);
		refresh();
		echo();
		getnstr(linea, sizeof(linea));
		noecho();
		// Arreglo para el scroll
		getyx(stdscr,y,x);
		if (y==yMaxima-1)
			scroll(stdscr);

		if (strlen(linea)==0)
                	continue;

                char* cad = strtok(linea," ");
		try {
			if (strcmp(cad,"salir")==0)
			// Sale del interprete de comandos
				return;
			else if (strcmp(cad,"disco")==0) {
			// Cambia de unidad
				// Obtiene el número de unidad
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el número de disco";

				// Comprueba que la unidad sea válida
				long n = atoi(cad);
				if (!(sfv.activo(n)))
					throw "esa unidad no existe";

				// Actualiza la unidad y directorio actuales
				unidad = n;
				long dir = sfv.abrir(unidad, directorio, "/");
				sfv.cerrar(directorio);
				directorio = dir;
			} else if (strcmp(cad,"creadir")==0) {
			// Crea un directorio
				// Obtiene el nombre del directorio
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el nombre del directorio";

				// Crea el directorio
				long id = sfv.crear(unidad, directorio, cad,
							DIRECTORIO);

				// Cierra el directorio
				sfv.cerrar(id);
			} else if (strcmp(cad,"cd")==0) {
			// Cambia de directorio
				// Obtiene el nombre del directorio
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el nombre del directorio";

				// Actualiza el directorio actual
				long id = sfv.abrir(unidad,directorio,cad);
				sfv.cerrar(directorio);
				directorio = id;
			} else if (strcmp(cad,"poner")==0) {
			// Copia un fichero del SO anfitrión a Eaftios
				// Obtiene el nombre del fichero
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el nombre del fichero";

				// Abre el fichero en el SO anfitrión
				FILE* fte = fopen(cad,"rb");
				if (fte == NULL)
					throw "ese fichero no existe";

				// Obtiene el nombre del fichero sin la ruta
				char *cad2, *cad3;
				cad2 = strtok(cad, SEPARADOR);
				while (cad2) {
					cad3 = cad2;
					cad2 = strtok(NULL, SEPARADOR);
				}
				cad = cad3;

				// Crea el fichero en Eafitos
				long id = sfv.crear(unidad, directorio, cad,
							NORMAL);

				// Copia el fichero
				char dato;
				while (!feof(fte)) {
					fread(&dato,sizeof(char),1,fte);
					sfv.escribir(id, &dato, 1);
				}

				// Cierra los ficheros
				sfv.cerrar(id);
				fclose(fte);
			} else if (strcmp(cad,"obtener")==0) {
			// Copia un fichero Eafitos al SO anfitrión
				// Obtiene el nombre del fichero
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el nombre del fichero";

				// Abre el fichero en Eafitos
				long fte = sfv.abrir(unidad, directorio, cad);

				// Obtiene el nombre del fichero sin la ruta
				char cad2[100];
				strcpy(cad2, cad);
				char *cad3, *cad4;
				cad3 = strtok(cad2, "/");
				while (cad3) {
					cad4 = cad3;
					cad3 = strtok(NULL, "/");
				}
				cad = cad4;

				// Crea el fichero en el SO anfitrión
				FILE* dtno = fopen(cad,"wb");

				// Copia el fichero
				char dato;
				long nDatos;
				while (1) {
					nDatos = sfv.leer(fte, &dato, 1);
					if (!nDatos)
						break;
					fwrite(&dato,sizeof(char),1,dtno);
				}

				// Cierra los ficheros
				sfv.cerrar(fte);
				fclose(dtno);
			} else if (strcmp(cad,"borrar")==0) {
			// Borra un fichero
				// Obtiene el nombre del fichero
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el nombre del fichero";

				// Borra el fichero
				sfv.borrar(unidad,directorio,cad);
			} else if (strcmp(cad,"dir")==0) {
			// Lista el contenido de un directorio
				InfoFichero info;
				sfv.saltar(directorio, 0, PRINCIPIO);

				printw("Nombre              \tTipo\tTamaño\n");
				printw("------              \t----\t------\n");
				while (sfv.infoFichero(directorio, info)) {
					printw("%-20s\t", info.nombre);
					if (info.tipo==NORMAL)
						printw("\t");
					else if (info.tipo==DIRECTORIO)
						printw("<DIR>\t");
					else
						printw("<???>\t");
					printw("%d\n", info.tamano);
				}
				refresh();
			} else if (strcmp(cad,"ver")==0) {
			// Muestra el contenido de un fichero
				// Obtiene el nombre del fichero
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el nombre del fichero";

				// Abre el fichero
				long id = sfv.abrir(unidad,directorio,cad);

				// Escribe el fichero en pantalla
				char car;
				while (sfv.leer(id, &car, 1)==1)
					addch(car);
				refresh();

				// Cierra el fichero
				sfv.cerrar(id);
			} else if (strcmp(cad,"compilar")==0) {
			// Compila un fichero
				// Obtiene el nombre del fichero
				cad = strtok(NULL," ");
				if (cad == NULL)
					throw "falta el nombre del fichero";

				// Genera el nombre del fichero ejecutable
				char obj[100];
				strcpy(obj, cad);
				strcat(obj, ".exe");

				// Compila el fichero
				Entorno::ensamblador.analizar(cad, obj, unidad,
								directorio);
			} else {
			// Ejecuta el programa
				// Crea el hilo y el proceso
				Nucleo::manejadorProcesos.ejecutar(unidad,
							directorio, cad);

				// Lo ejecuta
				MaqVirtual::cpu.ejecutar();
			}
		}
		catch(char* cad) {
			printw("Error, %s\n", cad);
			refresh();
		}
	}
}
