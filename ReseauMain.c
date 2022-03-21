#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Reseau.h"
#include "Chaine.h"
#include "ArbreQuat.h"
#include "Hachage.h"
#include "SVGwriter.h"
#define MAX 5000 // Valeur maximale pour la fonction


int main(){
    srand(time(NULL));
    FILE* f = fopen("00014_burma.cha","r");
    if(f==NULL){
        printf("Le fichier de lecture n'a pas pu s'ouvrir\n");
        exit(1);
    }
    FILE* f_write = fopen("Time_Exec.txt","w");
    if(f==NULL){
        printf("Le fichier de donnees n'a pas pu s'ouvrir\n");
        exit(1);
    }
    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu;
    //Creation des trois types de reseaux
    Reseau* r_Hachage;
    Reseau* r_Liste;
    Reseau* r_Arbre;

	Chaines* c= lectureChaines(f);

    for (int i = 500;i<MAX; i+=500) {

        //Reseau de Hachage

        temps_initial = clock();
        //Nombre de ‘‘ticks’’ consommees par le programme jusqu’ici
        r_Hachage = reconstitueReseauHachage(c,i);
        temps_final = clock();
        //Nombre de ‘‘ticks’’ consommees par le programme jusqu’ici
        //On convertit les ‘‘ticks’’ consommees par fct en secondes 
        temps_cpu =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;
        fprintf(f_write, "%d %f ",i,temps_cpu);

        //Reseau de liste

        temps_initial = clock();
        //Nombre de ‘‘ticks’’ consommees par le programme jusqu’ici
        r_Liste = reconstitueReseauListe(c);
        temps_final = clock();
        //Nombre de ‘‘ticks’’ consommees par le programme jusqu’ici
        //On convertit les ‘‘ticks’’ consommees par fct en secondes 
        temps_cpu =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;
        fprintf(f_write, "%f ",temps_cpu);

        //Reseau d'Arbre

        temps_initial = clock();
        //Nombre de ‘‘ticks’’ consommees par le programme jusqu’ici
        r_Arbre = reconstitueReseauArbre(c);
        temps_final = clock();
        //Nombre de ‘‘ticks’’ consommees par le programme jusqu’ici
        //On convertit les ‘‘ticks’’ consommees par fct en secondes 
        temps_cpu =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;
        fprintf(f_write, "%f\n",temps_cpu);
    }
    //Liberation des reseaux en memoire 
    libererReseau(r_Arbre);
    libererReseau(r_Hachage);
    libererReseau(r_Liste);

    //Fermeture des fichiers
    fclose(f);
    fclose(f_write);

    return 0;
}