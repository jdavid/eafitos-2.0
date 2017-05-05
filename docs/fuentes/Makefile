TEX=latex

FUENTES	= personal.sty \
	  manual.tex prefacio.tex \
	  usuario/uso.tex usuario/programa.tex \
	  interior/intro.tex \
	  interior/mv.tex \
	  interior/nucleo.tex \
	  interior/memoria.tex interior/hilos.tex \
	  interior/es.tex interior/sf.tex \
	  entorno/entorno.tex entorno/ensambla.tex \
	  apendice/fuentes.tex apendice/semaforo.tex \
	  apendice/gpl.tex apendice/gpl-es.tex

FIGURAS = figuras/eafitos.eps \
	  figuras/maq_vir.eps figuras/log_fis.eps figuras/disco.eps \
	  figuras/llamadas.eps \
	  figuras/hilo.eps figuras/ejec99.eps \
	  figuras/controla.eps figuras/dispos.eps \
	  figuras/sfv.eps figuras/sfclases.eps figuras/eafitsf.eps \
	  figuras/ensambla.eps figuras/afd.eps


%.eps : %.fig
	fig2dev -L ps $< $@

%.eps : %.dot
	dot -Tps $< -o $@

dvi: $(FIGURAS) $(FUENTES)
	$(TEX) manual.tex
	$(TEX) manual.tex
	touch dvi


ps: dvi
	dvips -o manual.ps manual.dvi
	touch ps

pdf: dvi
	dvipdf manual.dvi
	touch pdf

figuras: $(FIGURAS)
	

all: dvi ps pdf


clean:
	rm -f *.log *.aux *.toc *.lof *.lot *.dvi *.ps *.pdf
	rm -f figuras/*.eps figuras/*.bak
	rm -f dvi ps pdf
