CPPFLAGS=-I./include -D_LINUX_
CXXFLAGS=-g -Wall

OBJECTS = mv/mv.o						\
	mv/memfis.o mv/mmu.o mv/cpu.o				\
	mv/teclado.o mv/pantalla.o mv/disco.o			\
	nucleo/nucleo.o						\
	nucleo/memoria.o					\
	nucleo/hilo.o nucleo/proceso.o nucleo/procesos.o	\
	nucleo/cola.o						\
	nucleo/ejec/ejec99.o					\
	nucleo/es/controla.o					\
	nucleo/es/bloque.o nucleo/es/caracter.o			\
	nucleo/es/cbloque.o nucleo/es/cdisco.o			\
	nucleo/es/ccar.o nucleo/es/cteclado.o nucleo/es/cpantall.o \
	nucleo/sf/sfv.o nucleo/sf/flocal.o			\
	nucleo/sf/eafitsf.o nucleo/sf/efichero.o		\
	entorno/entorno.o entorno/ic.o entorno/ensambla.o	\
	eafitos2.o

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

dinamico: $(OBJECTS)
	$(CXX) $(OBJECTS) -o eafitos2 -lncurses

estatico: $(OBJECTS)
	$(CXX) $(OBJECTS) -o eafitos2-estatico -lncurses -static
	strip eafitos2-estatico

clean:
	rm -f eafitos2 eafitos2-estatico
	rm -f $(OBJECTS)
#	rm -f $(OBJECTS:.o=.d)


# Para generar dependencias automáticamente
#%.d: %.cc
#	$(SHELL) -ec '$(CXX) -M $(CPPFLAGS) $< \
#		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
#		[ -s $@ ] || rm -f $@'
  

#include $(OBJECTS:.o=.d)
