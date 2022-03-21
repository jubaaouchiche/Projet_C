#include <stdlib.h>
#include <stdio.h>
#include "ArbreQuat.h"
#include "Struct_Liste.h"
#include "Struct_File.h"
#include "Chaine.h"
#include "Reseau.h"
#include "Graphe_H.h"

int main(){

    FILE* f= fopen("00014_burma.cha", "r");
	if (f==NULL){
		printf("Erreur d'ouverture de fichier lecture\n");
		exit(1);
	}

	Chaines* ch= lectureChaines(f);

    Reseau* r1 = reconstitueReseauArbre(ch);

    Graphe* g1 = creerGraphe(r1);

    reorganiseReseau(r1);
    return 0;
}