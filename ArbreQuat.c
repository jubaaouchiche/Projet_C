#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "Chaine.h"
#include "ArbreQuat.h"

void chaineCoordMinMax(Chaines* C, double* x_min, double* y_min, double* x_max, double* y_max){
    *x_min = 9999999999999;
    *y_min = 9999999999999;
    CellPoint* p;
    CellChaine* ch= C->chaines;
    for(; ch; ch=ch->suiv){
        p = ch->points;
        for(; p; p=p->suiv){
        	if (p->x< *x_min) *x_min= p->x;
            if (p->x> *x_max) *x_max= p->x;
        	if (p->y< *y_min) *y_min= p->y;
        	if (p->y > *y_max) *y_max= p->x;
        }
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    //Allocation et creation de l'ArbreQuat
	ArbreQuat* a= (ArbreQuat*) malloc(sizeof(ArbreQuat));
	if (a==NULL){
        printf("Problème d'allocation d'ArbreQuat\n");
        return NULL;
    }
    //Initialisation de l'ArbreQuat
    a->xc= xc;
    a->yc= yc;
    a->coteX= coteX;
    a->coteY= coteY;
    a->ne = NULL;
    a->se = NULL;
    a->so = NULL;
    a->no = NULL;
    a->noeud = NULL;
    return a;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    if (n==NULL){
        printf("Noeud vide\n");
        return;
    }
    // Arbre vide
    if (*a==NULL){
        double xc;
        double coteX= parent->coteX*0.5;
        double coteY= parent->coteY*0.5;
        // On cree l'arbre avec le noeud et on insere chez le parent
        // arbre à l'ouest
        if (n->x< parent->xc){
            xc= parent->xc*0.5;
            // arbre sud ouest
            if (n->y< parent->yc) *a=creerArbreQuat(xc, parent->yc*0.5, coteX, coteY);
            // arbre nord ouest
            else creerArbreQuat(xc, parent->yc*1.5, coteX, coteY);
        }
        // arbre à l'est
        else{ 
            xc= parent->xc*1.5;
            // cote sud est
            if (n->y< parent->yc) *a=creerArbreQuat(xc, parent->yc*0.5, coteX, coteY);
            // cote nord est
            else *a= creerArbreQuat(xc, parent->yc*1.5, coteX, coteY);
        }
        if (*a) (*a)->noeud= n;
        return;
    }
    // feuille
    if ((*a)->noeud){
        Noeud* n1= (*a)->noeud;
        (*a)->noeud=NULL;
        if (n1->x< parent->xc){
            if (n1->y< parent->yc) insererNoeudArbre(n1, &((*a)->so), *a);
            else insererNoeudArbre(n1, &((*a)->no), *a);
        }
        else{ 
            if (n1->y< parent->yc) insererNoeudArbre(n1, &((*a)->se), *a);
            else insererNoeudArbre(n1, &((*a)->ne), *a);
        }
    }
    // Cellule interne
    if (n->x< parent->xc){
        if (n->y< parent->yc) insererNoeudArbre(n, &((*a)->so), *a);
        else insererNoeudArbre(n, &((*a)->no), *a);
    }
    else{ 
        if (n->y< parent->yc) insererNoeudArbre(n, &((*a)->se), *a);
        else insererNoeudArbre(n, &((*a)->ne), *a);
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    //Arbre vide
    if (*a==NULL){
        Noeud* n= alloueNoeud(++R->nbNoeuds, x, y);
        if (n==NULL) return NULL;
        insererNoeudArbre(n, a, parent);
        // On met en tête, le noeud de la liste des noeuds
        CellNoeud* cn= alloueCellNoeud();
        if (cn==NULL) return NULL;
        cn->nd= n;
        cn->suiv= NULL;
        return n;
    }
    // feuille
    if ((*a)->noeud != NULL){
        //on verifie si le noeud actuelest le noeud correspondante
        if ((*a)->noeud->x == x && (*a)->noeud->y == y) return (*a)->noeud;
        else{
            Noeud* n= alloueNoeud(++R->nbNoeuds, x, y);
            n = rechercheCreeNoeudListe(R,x,y);
            insererNoeudArbre(n, a, parent);
            return n;
        }
    }
    // Cellule interne
    if ((*a != NULL) && ((*a)->noeud == NULL)){
        if (x< parent->xc){
            if (y< parent->yc) rechercheCreeNoeudArbre(R, &((*a)->so), parent, x, y);
            else rechercheCreeNoeudArbre(R, &((*a)->no), parent, x, y);
        }
        else if (y< parent->yc) rechercheCreeNoeudArbre(R, &((*a)->se), parent, x, y);
        else rechercheCreeNoeudArbre(R, &((*a)->ne), parent, x, y);
    }
    return NULL;
}

Reseau* reconstitueReseauArbre(Chaines* C){
    Reseau* result = alloueReseau();
    double x_min = 0, y_min = 0, x_max = 0, y_max = 0;
    chaineCoordMinMax(C,&x_min,&y_min,&x_max,&y_max);
    ArbreQuat* quat = creerArbreQuat((x_max+x_min/2),(y_max+y_min/2),x_max-x_min,y_max-y_min);
    //Reconstitution de l'arbre du Reseau
    for (CellChaine* chaine = C->chaines; chaine;chaine = chaine->suiv){
        //Creation et initialisation des Cellpoints du début et de la fin actuel
        CellPoint* points_prec = chaine->points;
        CellPoint* start = chaine->points;
        for (CellPoint* points = chaine->points; points; points = points->suiv){
            Noeud* n1 = rechercheCreeNoeudArbre(result,&quat,quat,points_prec->x,points_prec->y);
            Noeud* n2 = rechercheCreeNoeudArbre(result,&quat,quat,points->x,points->y);
            //Affichage des valeurs des deux noeuds en x est en y
            //Ajout de ces voisins respectives
            insererVoisin(n1,n2);

            //Reinitialisation de points_prec pour satisfaire les voisins pour les noeuds suivants
            points_prec = points;
        }
        Noeud* n_start = rechercheCreeNoeudArbre(result,&quat,quat,start->x,start->y);
        Noeud* n_prec = rechercheCreeNoeudArbre(result,&quat,quat,points_prec->x,points_prec->y);
        //Affichage des valeurs des deux noeuds en x est en y
        //Creation de la commodite entre les deux noeuds du début est le précendant actuel
        result->commodites = ajoutCommodite(result->commodites, n_start, n_prec);
    }
    return result;
}