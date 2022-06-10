#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "Chaine.h"
#include "Hachage.h"
#include <math.h>

double clef(double x,double y){
    //Fonction cle
	return y+(x+y)*(x+y+1)*0.5;
}

int fonctionHachage(int k, int M){
    //Fonction de Hachage initialisé
    int A = (sqrt(5)-1)/2;

    return floor(M*(k*A - (floor(k*A))));
}

TableHachage* alloueHachage(int m){
    //Allocation et creation de la Table de Hachage
    TableHachage* t= (TableHachage*) malloc(sizeof(TableHachage));
    if (t==NULL){
        printf("Erreur d'alllocation de TableHachage\n");
        return NULL;
    }
    //Allocation et creation des noeuds a l'interieur du Tableau de Hachage
    t->noeuds= (CellNoeud**) malloc(sizeof(CellNoeud*)*m);
    if (t->noeuds==NULL){
        printf("Erreur d'alllocation de TableHachage\n");
        free(t);
        return NULL;
    }
    //Initialisation des valeurs
    t->m=m;
    t->n=0;
    return t;
}

Noeud* rechercheCreeNoeudHachage(Reseau *R, TableHachage* H, double x, double y){
    int indice = fonctionHachage(H->m,clef(x,y));
    CellNoeud *C = (H->noeuds)[indice];
    //Recherche du noeud dans la liste
    for(;C;C = C->suiv){
        Noeud* n = C->nd;
        if((n->x == x) && (n->y == y)){
            return n;
        }
    }

    //Creation + Insertion du noeud dans la liste de noeud, donc dans la table de Hachage
    Noeud* c = (Noeud *)(malloc(sizeof(Noeud)));
    CellNoeud* N = (CellNoeud*)(malloc(sizeof(CellNoeud)));

    c->x = x;
    c->y = y;
    c->num = (R->nbNoeuds)+1;
    R->nbNoeuds++;

    //Allocation et creation des voisins pour le noeud
    c->voisins = (CellNoeud *)(malloc(sizeof(CellNoeud)));
    if(c->voisins==NULL){
        printf("Erreur : Allocation liste des voisins\n");
        return NULL;
    } 

    //Ajout des Noeuds dans la Table de Hachage
    (H->noeuds)[indice] = ajoutNoeud((H->noeuds)[indice], c) ;
    N->nd = c;
    N->suiv = C;
    R->noeuds = N;
    //Retourne le Noeud de Hachage
    return c;
}

Reseau* reconstitueReseauHachage(Chaines* C, int M){
    //Allocation et creation de la Table de Hachage
	TableHachage* H= alloueHachage(M);
    if (H==NULL) return NULL;
    //Allocation et creation du Reseau
	Reseau* r= alloueReseau();
    if (r==NULL){
        free(H->noeuds);
        free(H);
        return NULL;
    }
    //Initialisation du Table de Hachage
	H->m= M;
	H->n= 0;
	r->gamma= C->gamma;
    //Reconstitution du Reseau de Hachage
    for(CellChaine* ch= C->chaines; ch!=NULL; ch=ch->suiv){
    	CellPoint* p= ch->points;
    	Noeud* n= rechercheCreeNoeudHachage(r, H, p->x, p->y);
    	Noeud* n2= n;
    	if (n){
    		if (p->suiv){
        		Noeud* n1= rechercheCreeNoeudHachage(r, H, (p->suiv)->x, (p->suiv)->y);
        		// Ajout du noeud dans les voisins de n
        		CellNoeud* v= ajoutNoeud(n->voisins, n1);
        		if (v) n->voisins= v;
        		else {
        			libererReseau(r);
        			return NULL;
        		}
        	}
    	}
        for(CellPoint* p= ch->points; (p->suiv)!=NULL; p=p->suiv){
            ////Allocation et creation du Noeud de Hachage
        	n= rechercheCreeNoeudHachage(r, H, (p->suiv)->x, (p->suiv)->y);
        	n2= n;
        	if (n){
        		Noeud* n1= rechercheCreeNoeudHachage(r, H, p->x, p->y);
        		// Ajout du noeud dans les voisins de n
        		CellNoeud* v= ajoutNoeud(n->voisins, n1);
        		if (v) n->voisins= v;
        		// On verifie s'il y a autre point
        		if ((p->suiv)->suiv){
        			n1= rechercheCreeNoeudHachage(r, H, (p->suiv->suiv)->x, (p->suiv->suiv)->y);
        			// Ajout du noeud dans les voisins de n
        			CellNoeud* v= ajoutNoeud(n->voisins, n1);
        			if (v) n->voisins= v;
        			else {
        			libererReseau(r);
        			return NULL;
        			}
        		}
        	}
        }
        CellCommodite* com= ajoutCommodite(r->commodites, n2, n);
        if (com) r->commodites = com;
	}
	return r;
}