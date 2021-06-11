#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "survie.h"
#include "menu.h"
#include "high_score.h"
#include "outils.h"

//Fonction de tri de liste high score

int read_file_high_score_outils(char *chemin,char *nom_high_score_survie,int score_survie,liste *pFirst){
	FILE *fichier;
	char ligne[128];

	fichier = fopen(chemin, "a");
	fclose(fichier);

	fichier = fopen(chemin, "r");

	if(strcmp(&nom_high_score_survie[0],"") != 0){
		tri_high_score_outils(&pFirst,nom_high_score_survie,score_survie);
	}else if(strcmp(&nom_high_score_survie[0],"") == 0){
		tri_high_score_outils(&pFirst,"Joueur inconnu",score_survie);
	}

	while(fgets(ligne, sizeof(ligne), fichier) != NULL){
		int len = strlen(ligne);

		char *nom = strtok(ligne,":");
		int score = strtol(strtok(NULL,":"),NULL,10);

		tri_high_score_outils(&pFirst,nom,score);
	}

	fclose(fichier);

	sup_liste_high_score_outils(chemin);
	write_liste_high_score_outils(pFirst,chemin);
	free_liste_high_score_outils(&pFirst);

	switch_screen_survie();
}

int tri_high_score_outils(liste **pFirst,char *nom,int score){
	liste *temp = *pFirst;

	liste *a_ajouter = (liste *)malloc(sizeof(liste));
	a_ajouter->score = score;

	a_ajouter->nom = (char *)malloc((strlen(nom))*sizeof(char));
	strcpy(a_ajouter->nom,nom);

	a_ajouter->pNext = NULL;

	if(pFirst == NULL)return 0;
	
	if(*pFirst == NULL){
		*pFirst = a_ajouter;
		return 0;
	}

	if((*pFirst)->score <= a_ajouter->score){
		a_ajouter->pNext = (*pFirst);
		*pFirst = a_ajouter;
		return 0;
	}

	while(temp->pNext != NULL){
		if(temp->pNext->score <= a_ajouter->score){
			a_ajouter->pNext = temp->pNext;
			temp->pNext = a_ajouter;
			return 0;
		}
		temp = temp->pNext;
	}

	if(temp->score >= a_ajouter->score){
		temp->pNext = a_ajouter;
		return 0;
	}
}

int write_liste_high_score_outils(liste *pFirst,char *chemin){
	liste *pTemp = pFirst;
	int compteur = 0;

	while(pTemp != NULL && compteur <= 4){
		write_file_outils(pTemp->nom,pTemp->score,chemin);
		compteur++;
		pTemp=pTemp->pNext;
	}

	return 0;
}

int sup_liste_high_score_outils(char *chemin){
	FILE *fichier;
	fichier = fopen (chemin,"w");
	fclose(fichier);
}

int write_file_outils(char *nom, int score,char *chemin){
	FILE *fichier;
	fichier = fopen (chemin,"a");
	fprintf (fichier,"%s:%d \n",nom,score);
	fclose(fichier);
}

int free_liste_high_score_outils(liste **pFirst){
	liste *temp_un = *pFirst;
	liste *temp_deux = NULL;

	while(temp_un != NULL){
		temp_deux = temp_un->pNext;
		free(temp_un);
		temp_un = temp_deux;
	}

	*pFirst = NULL;

	return 0;
}

/*--------------------------------------------------------------------*/

