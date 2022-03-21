#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h"

void libererChaines(Chaines* C){
    for(CellChaine* ch = C->chaines; ch;ch = ch->suiv){
        for(CellPoint* chp = ch->points; chp; chp=chp->suiv){
            //Liberation des points dans la chaine
            free(chp);
        }
        //Liberation des Cellules chaines
        free(ch);
    }
    //Liberation de la chaine
    free(C);
}

/*Allocation, remplissage d'une structure Chaine a partir d'un fichier*/
Chaines* lectureChaines(FILE* f){
    Chaines* reseau = (Chaines*)(malloc(sizeof(Chaines)));
    FILE* fi = f;

    if(reseau==NULL){
        printf("Erreur d'allocation !: Chaines\n");
        return NULL;
    }
    if(fi==NULL){
        printf("Erreur d'allocation !: ouverture fichier\n");
        return NULL;
    }
    int nbchaine = 0;
    char buffer[265];

    //Extraction des données voulues a partir du fic
    fgets( buffer , 265 , fi);
    sscanf( buffer , "NbChain: %d\n", &nbchaine );
    reseau->nbChaines = nbchaine ;

    int gamma = 0 ;
    fgets( buffer , 265 , f);
    sscanf( buffer , "Gamma: %d\n", &gamma );
    reseau->gamma = gamma ;
    reseau->chaines= NULL;

    // On prend ligne par ligne, on cree la chaine et sses points
    while(fgets(buffer,265,fi) != NULL){
        int n=0, num=0;
        // On alloue la chaine et on verifie si c'est allouée
        CellChaine* chaine= (CellChaine*)malloc(sizeof(CellChaine));
        if (chaine==NULL) {
            printf("Erreur d'allocation: CellChaine\n");
            return NULL;
        }
        // On met son numéro et on l'ajoute en tête de liste de chaine
        sscanf(buffer,"%d  %d ",&num, &n);
        chaine->numero = num;
        chaine->suiv= reseau->chaines;
        reseau->chaines= chaine;
        // On crée les points de la chaine
        char* p= buffer+4;
        for (int i=0; i<n; i++){
            // on récupère x et y du point
            double x,y;
            char s1[6], s2[6];
            sscanf(p, "%s %s", s1, s2);
            sscanf(s1, "%lf", &x);
            sscanf(s2, "%lf", &y);
            // on crée le point (x,y)
            CellPoint* point= (CellPoint*) malloc(sizeof(CellPoint));
            if (point== NULL){
                printf("Erreur d'allocation\n");
                return NULL;
            }
            point->x= x; point->y= y;
            // on l'ajoute en tête de la liste de points
            point->suiv= chaine->points;
            chaine->points= point;

            p= p+strlen(s1)+strlen(s2)+2;
        }
    }
    return reseau;
}
    

// Ecrit dans un fichier le contenu d'une Chaines
void ecrireChaines(Chaines* C, FILE* f){
    if (f==NULL){
        printf("ecrireChaine(): Pointeur de fichier vide\n");
        return;
    }
    if (C==NULL){
        printf("ecrireChaine(): Chaines vide");
        return;
    }
    // Ecrit dans le Fichier, le nombre de chaine et gamma
    fprintf(f, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);
    for(CellChaine* ch= C->chaines; ch; ch=ch->suiv){
        int nbPoints=0;
        for(CellPoint* p= ch->points; p; p=p->suiv){
            nbPoints++;}
        fprintf(f, "%d %d ", ch->numero, nbPoints);
        for(CellPoint* p= ch->points; p; p=p->suiv)
            fprintf(f, "%.2lf %.2lf ", p->x, p->y);
        fprintf(f, "\n");
    }
}


void afficheChainesSVG(Chaines* C, char* nomInstance){
    double max_x=0,max_y=0,min_x=1e6,min_y=1e6;
    CellChaine* ccour = (CellChaine*)(malloc(sizeof(CellChaine)));
    CellPoint* pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    for(;ccour;ccour = ccour->suiv){
         pcour=ccour->points;
         for(;pcour;pcour = pcour->suiv){
            if (max_x<pcour->x) max_x=pcour->x;
            if (max_y<pcour->y) max_y=pcour->y;
            if (min_x>pcour->x) min_x=pcour->x;
            if (min_y>pcour->y) min_y=pcour->y;  
        }
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    for(;ccour;ccour = ccour->suiv){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-min_x)/(max_x-min_x),500*(pcour->y-min_y)/(max_y-min_y)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        for(;pcour;pcour = pcour->suiv){
            SVGline(&svg,500*(precx-min_x)/(max_x-min_x),500*(precy-min_y)/(max_y-min_y),
            500*(pcour->x-min_x)/(max_x-min_x),500*(pcour->y-min_y)/(max_y-min_y));
            SVGpoint(&svg,500*(pcour->x-min_x)/(max_x-min_x),500*(pcour->y-min_y)/(max_y-min_y));
            precx=pcour->x;
            precy=pcour->y;
        }
    }
    SVGfinalize(&svg);
}

double longueurChaine(CellChaine* c){
    double l = 0.0;
    CellPoint* ch = (CellPoint*)(malloc(sizeof(CellPoint)));
    CellPoint* ch1 = (CellPoint*)(malloc(sizeof(CellPoint)));
    ch = c->points;
    ch1 = ch->suiv;
    double x1, y1, x2, y2;
    while(ch1){
        x1 = ch->x;
        y1 = ch->y;
        x2 = ch1->x;
        y2 = ch1->y;
        ch = ch->suiv;
        ch1 = ch1->suiv;
        l+=sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
    }
    return l;
}

double longueurTotale(Chaines* c){
    double sum = 0;
    for (CellChaine* ch = c->chaines;ch;ch = ch->suiv){
        //Somme des points dans la chaine pour avoir sa longueur totale
        sum += longueurChaine(ch);
    }
    return sum;
}

int comptePointsTotal(Chaines* C){
    //Nombre de Points totale initialiser
    int result = 0;
    
    for (CellChaine* ch = C->chaines;ch;ch = ch->suiv){
        for (CellPoint* chp = ch->points;chp;chp = chp->suiv){
            result++;
        }
    }
    return result;
}

Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int x_max,
int y_max){
    //Allocation et creation de la Chaine resultante
    Chaines* result = (Chaines*)malloc(sizeof(Chaines));
    result->nbChaines = nbChaines;
    //Valeur aleatoire choisie pour gamma car la fonction pour avoir gamma est inconnue
    result->gamma = rand()%5; 
    CellChaine* chaines;
    CellPoint* points;
    //Reconstitution de la Chaine resultante
    for(int numero = 0;numero<nbChaines;numero++){
        chaines = malloc(sizeof(CellChaine));
        chaines->numero = numero;
        //Creation des points pour la chaine resultante
        for (int nbpoints = 0; nbpoints<nbPointsChaine; nbpoints++){
            points = malloc(sizeof(CellPoint));
            chaines->points = points;
            //Initialisation des valeurs de x et y
            points->x = (double) (rand()%(x_max+1));
            points->y = (double) (rand()%(y_max+1));
            points->suiv = chaines->points;
            chaines->points = points;
        }
        points = NULL;
        result->chaines = chaines;
        chaines->suiv = result->chaines;
    }
    chaines = NULL;
    return result;
}