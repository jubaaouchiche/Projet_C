CFLAGS = -g -Wall -Wextra -pedantic -lm -ggdb
CC = gcc

PROGRAMS = ChaineMain ReseauMain GrapheMain

.PHONY:	all clean

.SUFFIX:

all: $(PROGRAMS)

Chaine.o : Chaine.c Chaine.h
	$(CC) $(CFLAGS) -c Chaine.c

SVGwriter.o : SVGwriter.c SVGwriter.h
	$(CC) $(CFLAGS) -c SVGwriter.c

Reseau.o : Reseau.c Reseau.h
	$(CC) $(CFLAGS) -c Reseau.c

ArbreQuat.o : ArbreQuat.c ArbreQuat.h
	$(CC) $(CFLAGS) -c ArbreQuat.c

Hachage.o : Hachage.c Hachage.h
	$(CC) $(CFLAGS) -c Hachage.c

Struct_File.o : Struct_File.c Struct_File.h
	$(CC) $(CFLAGS) -c Struct_File.c

Struct_Liste.o : Struct_Liste.c Struct_Liste.h
	$(CC) $(CFLAGS) -c Struct_Liste.c

Graphe_H.o : Graphe_H.c Graphe_H.h
	$(CC) $(CFLAGS) -c Graphe_H.c

ChaineMain.o : ChaineMain.c Chaine.h
	$(CC) $(CFLAGS) -c ChaineMain.c

ReseauMain.o : ReseauMain.c Reseau.h Chaine.h Hachage.h 
	$(CC) $(CFLAGS) -c ReseauMain.c

GrapheMain.o : GrapheMain.c Struct_Liste.h Struct_File.h Chaine.h Reseau.h Graphe_H.h ArbreQuat.h
	$(CC) $(CFLAGS) -c GrapheMain.c

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o Reseau.o
	$(CC) $(CFLAGS) -o ChaineMain ChaineMain.o Chaine.o SVGwriter.o Reseau.o ArbreQuat.o Hachage.o

ReseauMain : ReseauMain.o Chaine.o Reseau.o ArbreQuat.o Hachage.o SVGwriter.o
	$(CC) $(CFLAGS) -o ReseauMain ReseauMain.o Chaine.o Reseau.o ArbreQuat.o Hachage.o SVGwriter.o

GrapheMain : GrapheMain.o Struct_Liste.o Struct_File.o Chaine.o Reseau.o Graphe_H.o SVGwriter.o ArbreQuat.o
	$(CC) $(CFLAGS) -o GrapheMain GrapheMain.o Struct_Liste.o Struct_File.o Chaine.o Reseau.o Graphe_H.o SVGwriter.o ArbreQuat.o

clean:
	rm -f *.o *~ $(PROGRAMS)