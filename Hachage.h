#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include "Reseau.h"
#include "Chaine.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct table{
    int m;
    int n;
    CellNoeud** noeuds;
} TableHachage;

double clef(double x,double y);

int fonctionHachage(int k, int M);

TableHachage* alloueHachage(int m);

Noeud* rechercheCreeNoeudHachage(Reseau *R, TableHachage* H, double x, double y);

Reseau* reconstitueReseauHachage(Chaines* C, int M);

#endif