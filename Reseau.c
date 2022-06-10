#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "SVGwriter.h"

Reseau* alloueReseau(){
    Reseau* r= (Reseau*) malloc(sizeof(Reseau));
    if (r==NULL){
        printf("Erreur d'alllocation du Réseau\n");
        return NULL;
    }
    r->nbNoeuds = 0;                   
    r->gamma = 0;                      
    r->noeuds = alloueCellNoeud();              
    r->commodites = NULL;
    return r;
}

Noeud* alloueNoeud(int num, double x, double y){
    Noeud* n= (Noeud*) malloc(sizeof(Noeud));
    if (n==NULL){
        printf("Erreur d'alllocation du Noeud\n");
        exit(1);
    }
    n->num= num;
    n->x= x;
    n->y= y;
    n->voisins = NULL;
    return n;
}

CellNoeud* alloueCellNoeud(){
    CellNoeud* c= (CellNoeud*) malloc(sizeof(CellNoeud));
    if (c==NULL){
        printf("Erreur d'alllocation du Réseau\n");
        exit(1);
    }
    c->nd = alloueNoeud(0,0,0);
    c->suiv = NULL;
    return c;
}

//Exercice 2, q1
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    CellNoeud *C = R->noeuds;
    //Recherche du noeud dans la liste
    while(C){
        Noeud* n = C->nd;
        if((n->x == x) && (n->y == y)){
            return n;
        }
        C = C->suiv;
    }
    //Creation + Insertion du noeud dans la liste de noeud, donc dans le Réseau
    Noeud* c = alloueNoeud((R->nbNoeuds)+1, x, y);
    if (c==NULL) return NULL;
    CellNoeud* N = alloueCellNoeud();
    if (N==NULL){
        free(c);
        return NULL;
    }
    R->nbNoeuds++;
    N->nd = c;
    N->suiv = C;
    R->noeuds = N;
    return c;
}

// Ajoute en tête de la liste, un noeud
CellNoeud* ajoutNoeud(CellNoeud* C, Noeud* n){
    // Si le noeud existe, on ajoute pas
    for (CellNoeud* c= C; c; c=c->suiv)
    	if (c->nd==n) return C;
    // On met en tête, le noeud de la liste des noeuds
    CellNoeud* cn= alloueCellNoeud();
    if (cn==NULL) return C;
    cn->nd= n;
    cn->suiv= C;
    return cn;
}

// Ajoute en tête de la liste, une nouvelle commodite de n1 et n2
CellCommodite* ajoutCommodite(CellCommodite* C, Noeud* n1, Noeud* n2){
	CellCommodite* c= (CellCommodite*) malloc(sizeof(CellCommodite));
	if (c==NULL){
		printf("Erreur d'allocation de Commodite\n");
		return NULL;
	}
	c->extrA=n1; c->extrB= n2;
	c->suiv= C;
	return c;
}

// Reconstitue en Réseau la Chaines
Reseau* reconstitueReseauListe(Chaines* C){
	Reseau* r= alloueReseau();
    if (r==NULL) return NULL;
	r->gamma= C->gamma;
    for(CellChaine* ch= C->chaines; ch; ch=ch->suiv){
    	// 
    	CellPoint* p= ch->points;
    	Noeud* n= rechercheCreeNoeudListe(r, p->x, p->y);
    	Noeud* n2= n;
    	if (n){
    		if (p->suiv){
        		Noeud* n1= rechercheCreeNoeudListe(r, (p->suiv)->x, (p->suiv)->y);
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
        	n= rechercheCreeNoeudListe(r, (p->suiv)->x, (p->suiv)->y);
        	n2= n;
        	if (n){
        		Noeud* n1= rechercheCreeNoeudListe(r, p->x, p->y);
        		// Ajout du noeud dans les voisins de n
        		CellNoeud* v= ajoutNoeud(n->voisins, n1);
        		if (v) n->voisins= v;
        		// On verifie s'il y a autre point
        		if ((p->suiv)->suiv !=NULL){
        			n1= rechercheCreeNoeudListe(r, (p->suiv->suiv)->x, (p->suiv->suiv)->y);
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
        if (com) r->commodites= com;
	}
	return r;
}
// Libère la liste de Noeuds
void libererNoeuds(CellNoeud* C){
    for(;C;C = C->suiv){
        free(C->nd->voisins);
        free(C->nd);}
    free(C);
}

// Libère la liste de Commodités
void libererCommodites(CellCommodite* C){
    for(;C;C = C->suiv){
		free(C->extrA);
    }
    free(C);
}

// Libère le réseau
void libererReseau(Reseau* R){
	libererNoeuds(R->noeuds);
	libererCommodites(R->commodites);
	free(R);
}

// Ecrit le contenu du raison
void ecrireReseau(Reseau *R, FILE *f){
	if (f==NULL){
		printf("Pointeur de Fichier vide\n");
		return;
	}
	if (R==NULL){
		printf("Pointeur de Reseau vide\n");
		return;
	}
	// On ecrit la description du Reseau
	fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\n", R->nbNoeuds, nbLiaisons(R));
	fprintf(f, "nbCommodites: %d\nNbLiaisons: %d\n\n", nbCommodites(R), R->gamma);

	// On ecrit les noeuds du réseau
	for (CellNoeud* cn= R->noeuds; cn; cn= cn->suiv)
		fprintf(f, "v %d %lf %lf\n", (cn->nd)->num, (cn->nd)->x, (cn->nd)->y);
	fprintf(f, "\n");

	// On ecrit les liaisons du réseau
	for (CellNoeud* cn= R->noeuds; cn; cn= cn->suiv){
		// on parcourt la liste des voisins de cn
        for (CellNoeud* c= (cn->nd)->voisins; c; c= c->suiv){
			// On ecrit la liaison dans le fichier
            if ((cn->nd)->num<(c->nd)->num) fprintf(f, "l %d %d\n", (cn->nd)->num, (c->nd)->num);
		}
	}
	fprintf(f, "\n");
	
	// On ecrit les commodites du réseau
	for (CellCommodite* c= R->commodites; c; c= c->suiv)
		fprintf(f, "k %d %d\n", (c->extrA)->num, (c->extrB)->num);
	fprintf(f, "\n");
}

// retourne le nombre de liaison dans le Réseau
int nbLiaisons(Reseau *R){
    int nb = 0;
    // On parcourt la liste des noeuds
    for(CellNoeud* ch = R->noeuds;ch;ch = ch->suiv){
        Noeud* n = ch->nd;
        // On parcourt la liste des voisins du noeud n
        for(CellNoeud* c = n->voisins; c!=NULL; c = c->suiv){
            if ((n->num<(c->nd)->num))
                nb++;
        }
    }
    return nb;
}


int nbCommodites(Reseau *R){
    int result = 0;
    for(CellCommodite* ch = R->commodites;ch;ch = ch->suiv){
        result++;
    }
    return result;
}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double max_x=0,max_y=0,min_x=1e6,min_y=1e6;
    courN = (CellNoeud*)malloc(sizeof(CellNoeud));

    for(courN=R->noeuds;courN;courN = courN->suiv){
        //Verification des mins et des maxs + Mise a jour si necessaire
        printf("%f",R->noeuds->nd->x);
        if (max_x<R->noeuds->nd->x) max_x=R->noeuds->nd->x;
        if (max_y<courN->nd->y) max_y=courN->nd->y;
        if (min_x>courN->nd->x) min_x=courN->nd->x;
        if (min_y>courN->nd->y) min_y=courN->nd->y;
    }
    //Creation de la SVG
    SVGinit(&svg,nomInstance,500,500);
    for(courN=R->noeuds;courN;courN = courN->suiv){
        //Creation des points pour la SVG
        SVGpoint(&svg,500*(courN->nd->x-min_x)/(max_x-min_x),500*(courN->nd->y-min_y)/(max_y-min_y));
        //Creation des lignes pour la SVG
        for(courv=courN->nd->voisins;courv;courv = courv->suiv){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-min_x)/(max_x-min_x),500*(courv->nd->y-min_y)/(max_y-min_y),
                500*(courN->nd->x-min_x)/(max_x-min_x),500*(courN->nd->y-min_y)/(max_y-min_y));
        }
    }
    SVGfinalize(&svg);
}

void insererVoisin(Noeud* n1,Noeud* n2){
    CellNoeud* cn1 = alloueCellNoeud();
    CellNoeud* cn2 = alloueCellNoeud();
    cn1->nd = n1;
    cn1->suiv = NULL;
    cn2->nd = n2;
    cn2->suiv = NULL;
    //n1->voisins = cn2;
    //n2->voisins = cn1;
}