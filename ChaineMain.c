#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"



int main(){
	FILE* f= fopen("00014_burma.cha", "r");
	if (f==NULL){
		printf("Erreur d'ouverture de fichier lecture\n");
		exit(1);
	}
	FILE* f1= fopen("exo1.2.txt", "w");
	if (f1==NULL){
		printf("Erreur d'ouverture de fichier ecriture\n");
		fclose(f);
		exit(1);
	}
	Chaines* ch= lectureChaines(f);
	ecrireChaines(ch, f1);
	afficheChainesSVG(ch, "exo1.3");

	Reseau* r= reconstitueReseauListe(ch);
	afficheReseauSVG(r, "exo3.3");
	libererReseau(r);
	libererChaines(ch);
	fclose(f);
	fclose(f1);
	return 0;
}