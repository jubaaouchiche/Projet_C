#include <stdlib.h>
#include <stdio.h>
#include "Struct_Liste.h"
#include "Struct_File.h"
#include "Chaine.h"
#include "Reseau.h"
#include "Graphe_H.h"

Graphe* alloueGraphe(){
    Graphe* result = (Graphe*)malloc(sizeof(Graphe));
    result->gamma = rand()%5;
    result->nbcommod = 0;
    result->nbsom = 0;
    result->T_commod = NULL;
    result->T_som = alloueSommet(0);
    return result;
}
Sommet** alloueSommet(int taille){
    Sommet** result = (Sommet**)malloc(sizeof(Sommet*)*taille);
    return result;
}
Cellule_arete* alloueArete(){
    Cellule_arete* result = (Cellule_arete*)malloc(sizeof(Cellule_arete));
    result->a = NULL;
    result->suiv = NULL;
    return result;
}
void ajoutVoisin(Sommet* s1,Sommet* s2){
    Arete* a1 = (Arete*)malloc(sizeof(Arete));
    a1->u = s1->num; a1->v = s2->num;

    Cellule_arete* c1 = alloueArete();
    Cellule_arete* c2 = alloueArete();

    c1->a = a1; c2->a = a1;

    c1->suiv = s1->L_voisin;
    c2->suiv = s2->L_voisin;
    s1->L_voisin = c1;
    s2->L_voisin = c2;
}
Sommet* celluleNoeudSommet(CellNoeud* cn){
    Sommet* result = malloc(sizeof(Sommet));
    result->num = cn->nd->num;
    result->L_voisin = NULL;
    result->x = cn->nd->x;
    result->y = cn->nd->y;
    return result;
}
Commod* cellCommoditeCommod(CellCommodite* cC){
    Commod* result = (Commod*)malloc(sizeof(Commod));
    double a = cC->extrA->num;
    result->e1 = a;
    result->e2 = cC->extrB->num;
    return result;
}
Graphe* creerGraphe(Reseau* r){
    //Initialisation du Graphe
    Graphe* result = alloueGraphe();
    Sommet* half;
    result->gamma = r->gamma;
    result->nbsom= r->nbNoeuds;
    //Initialisation des Commods par les Commodites
    result->nbcommod = nbCommodites(r);
    result->T_commod = malloc(sizeof(Commod)*result->nbcommod);
    CellCommodite* cC = r->commodites;
    for(int i = 0;i<result->nbcommod;i++){
        //(result->T_commod)[i] = *(cellCommoditeCommod(cC));
        if(cC->suiv) cC = cC->suiv;
        else break;
    }
    //Initialisation des sommets par les Noeuds
    result->T_som = alloueSommet(r->nbNoeuds);

    for(CellNoeud* cN = r->noeuds;cN;cN = cN->suiv){
        if((result->T_som)[cN->nd->num] == NULL){
            half = celluleNoeudSommet(cN);
            (result->T_som)[cN->nd->num] = half;
        }
        else{
            half = (result->T_som)[cN->nd->num];
        }
        for(CellNoeud* v = cN->nd->voisins; v; v = v->suiv){
            if ((result->T_som)[v->nd->num]==NULL){
                (result->T_som)[v->nd->num]= celluleNoeudSommet(v);
            }
            if ((result->T_som)[half->num]==0 && (result->T_som)[v->nd->num]==0){
                ajoutVoisin((result->T_som)[half->num],(result->T_som)[v->nd->num]);
            }
        }
    }
    return result;
}
int plusPetit_nbArretes(Reseau* r, Sommet* u, Sommet* v){
    //Initialisation
    int result = 0;
    Graphe* G = creerGraphe(r);
    int u1 = u->x;
    int u2 = u->y;
    //Creation d'un tableau pour les aretes visites
    int* visit =(int*) malloc(G->nbsom*sizeof(int));
    int i;
    for(i=0;i<G->nbsom;i++){
        visit[i]=0;
    }
    visit[u1]=1;
    visit[u2]=1;
    //Creation d'un file pour stocker les aretes interresante
    S_file* F = (S_file*)malloc(sizeof(S_file));
    Init_file(F);
    enfile(F,u1);
    enfile(F,u2);
    //Parcours dans le Graphe
    while(!( estFileVide(F))){
        int i=defile(F);
        printf("%d",i); //  ou  ajout  de u dans  une  liste
        Cellule_arete* cour1 = v->L_voisin;
        //Comparaison entre les deux aretes
        while(cour1!=NULL){
            int v1=cour1->a->u;
            int v2=cour1->a->v;
            if(visit[v1]==0){
                visit[v1]=1;
                enfile(F,v1);
            }
            if(visit[v2==0]){
                visit[v2]=1;
                enfile(F,v2);
            }
            cour1 = cour1->suiv;
            }
        }
    printf("\n ");
    //Calcul du nb d'aretes entre deux sommets
    for (int i = 0; visit[i]!=0; i++) {
        result++;
    }
    return result;
}
int plusPetit_nbArretes_Liste(Reseau* r, Sommet* u, Sommet* v){
    //Initialisation
    int result = 0;
    Graphe* G = creerGraphe(r);
    int u1 = u->x;
    int u2 = u->y;
    //Creation d'un tableau pour les aretes visites
    int* visit =(int*) malloc(G->nbsom*sizeof(int));
    int i;
    for(i=0;i<G->nbsom;i++){
        visit[i]=0;
    }
    visit[u1]=1;
    visit[u2]=1;
     //Creation d'une liste pour stocker les aretes interresante par chainage
    ListeEntier* L = (ListeEntier*)malloc(sizeof(ListeEntier));
    Cell_entier* c = (Cell_entier*)malloc(sizeof(Cell_entier));
    
    Init_Liste(L);
    ajoute_en_tete(L,u1);
    ajoute_en_tete(L,u2);
    //Parcours dans le Graphe
    while(estLileVide(L)){
        printf("%d",i); //  ou  ajout  de u dans  une  liste
        Cellule_arete* cour1 = v->L_voisin;
        //Comparaison entre les deux aretes
        while(cour1!=NULL){
            int v1=cour1->a->u;
            int v2=cour1->a->v;
            if(visit[v1]==0){
                visit[v1]=1;
                ajoute_en_tete(L,v1);
            }
            if(visit[v2==0]){
                visit[v2]=1;
                ajoute_en_tete(L,v2);
            }
            cour1 = cour1->suiv;
            }
        }
    printf("\n ");
    //Calcul du nb d'aretes entre deux sommets
    for (*L = c;c;c = c->suiv) {
        result++;
    }
    desalloue(L);
    return result;
}
int reorganiseReseau(Reseau* r){
    //Initialisation
    Graphe* g = creerGraphe(r);
    Sommet* s1;
    Sommet* s2;
    int** mat = (int**)malloc(sizeof(int*)*g->nbsom);
    for (int i = 0; i<g->nbsom;i++){
        mat[i] = malloc(sizeof(**mat) * g->nbsom);}
    //Creation et stockage des aretes par commodite et par aretes
    for(int i = 0;i<g->nbcommod;i++){
        for (int j = 0; j<g->nbcommod;j++) {
            s1 = g->T_som[i];
            s2 = g->T_som[j];
            mat[i][j] = plusPetit_nbArretes_Liste(r, s1, s2);
            //Fonction pour savoir si le nb d'aretes est au-dessus strict a gamma
            if(mat[i][j]>r->gamma) {free(mat); return 0;}
        }
    }
    return 1;
}