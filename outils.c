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
#include "versus.h"
#include "difficulte.h"
#include "ia.h"
#include "mode_ia.h"

//Fonction de tri de liste high score
void read_file_high_score_outils(char *chemin,char *nom_high_score_survie,int score_survie,liste *pFirst){
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

		char *nom = strtok(ligne,":");
		int score = strtol(strtok(NULL,":"),NULL,10);

		tri_high_score_outils(&pFirst,nom,score);
	}

	fclose(fichier);

	sup_liste_high_score_outils(chemin);
	write_liste_high_score_outils(pFirst,chemin);
	free_liste_high_score_outils(&pFirst);
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

	return 0;
}

void write_liste_high_score_outils(liste *pFirst,char *chemin){
	liste *pTemp = pFirst;
	int compteur = 0;

	while(pTemp != NULL && compteur <= 4){
		write_file_outils(pTemp->nom,pTemp->score,chemin);
		compteur++;
		pTemp=pTemp->pNext;
	}
}

void sup_liste_high_score_outils(char *chemin){
	FILE *fichier;
	fichier = fopen (chemin,"w");
	fclose(fichier);
}

void write_file_outils(char *nom, int score,char *chemin){
	FILE *fichier;
	fichier = fopen (chemin,"a");
	fprintf (fichier,"%s:%d \n",nom,score);
	fclose(fichier);
}

void free_liste_high_score_outils(liste **pFirst){
	liste *temp_un = *pFirst;
	liste *temp_deux = NULL;

	while(temp_un != NULL){
		temp_deux = temp_un->pNext;
		free(temp_un);
		temp_un = temp_deux;
	}

	*pFirst = NULL;
}

/*------------------------------------------*/

//Gestion des FPS pour les modes survie, versus et ia
void delay_game_outils(int etat_survie, int mode_de_jeu){
	int lastTicks_survie = 0;
	int delay_survie = 0;
	int maxFPS_survie = 60;

	if(mode_de_jeu == 0){
		if(etat_survie != 0) maxFPS_survie = 15; //Enleve le petit décalage de lancement
		if(etat_survie == 6) maxFPS_survie = 60;
	}else if(mode_de_jeu == 1){
		if(etat_survie != 1) maxFPS_survie = 15; //Enleve le petit décalage de lancement
		if(etat_survie >= 12) maxFPS_survie = 60;
	}

	lastTicks_survie = SDL_GetTicks();
	delay_survie = 1000/maxFPS_survie-SDL_GetTicks()+lastTicks_survie;
	if(delay_survie>0){
		SDL_Delay(delay_survie);
	}
}
/*------------------------------------------*/

//Gestion de la musique
void play_musique_outils(Mix_Music **music_de_fond_survie){
	*music_de_fond_survie = Mix_LoadMUS("./Musique/music.mp3");
	Mix_PlayMusic(*music_de_fond_survie, -1);
}

void play_explosion_outils(Mix_Chunk **explosion_survie){
	*explosion_survie = Mix_LoadWAV("./Musique/explosion.wav");
	Mix_PlayChannel(-1, *explosion_survie, 0);
}
/*------------------------------------------*/

//Gestion du tableau allocation memoire
void allocation_tableau_outils(int ***tableau_deplacement, int width_windows_survie, int height_windows_survie){
  
    (*tableau_deplacement) = (int **)malloc(width_windows_survie * sizeof(int *));

    for (int i=0; i<width_windows_survie; i++){
    	(*tableau_deplacement)[i] = (int *)malloc(height_windows_survie * sizeof(int));
    }

    for (int i = 0; i <  width_windows_survie; i++){
      for (int j = 0; j < height_windows_survie; j++){
         (*tableau_deplacement)[i][j] = 0;
      }
    }
}

void free_tableau_outils(int ***tableau_deplacement, int width_windows_survie){
	for (int i = 0; i < width_windows_survie ; ++i){
			free((*tableau_deplacement)[i]);
		}
	free((*tableau_deplacement));
}
/*------------------------------------------*/

//Fonction utilise afin de compter le nombre de caractères d'un string
int compte_nom_high_score_outils(char *nom_high_score_survie){
	int i = 0;
	while(strcmp(&nom_high_score_survie[i],"") != 0){
		i++;
	}
	return i;
}

/*------------------------------------------*/

//Fonction de transition pour fermer une fenetre ou changer de fenetre
void fermeture_sdl_outils(SDL_Window *fenetrePrincipale_survie, SDL_Renderer *renduPrincipale_survie, Mix_Chunk *explosion_survie, SDL_Surface *main_surface_survie, SDL_Texture *main_texture_survie,SDL_Surface *deuxieme_surface_versus,SDL_Texture *deuxieme_texture_versus,TTF_Font* font_general){
	SDL_DestroyRenderer(renduPrincipale_survie);
	SDL_DestroyWindow(fenetrePrincipale_survie);

	Mix_FreeChunk(explosion_survie);

	SDL_DestroyTexture(main_texture_survie);
	SDL_FreeSurface(main_surface_survie);

	if(deuxieme_surface_versus != NULL && deuxieme_texture_versus != NULL){
		SDL_DestroyTexture(deuxieme_texture_versus);
		SDL_FreeSurface(deuxieme_surface_versus);
	}

	TTF_CloseFont(font_general);

	SDL_Quit();

	exit(EXIT_SUCCESS);
}

void switch_screen_outils(SDL_Window *fenetrePrincipale_survie, SDL_Renderer *renduPrincipale_survie, Mix_Music *music_de_fond_survie, SDL_Surface *main_surface_survie, SDL_Texture *main_texture_survie, int etat_survie,SDL_Surface *deuxieme_surface_versus,SDL_Texture *deuxieme_texture_versus,TTF_Font* font_general){
	SDL_DestroyRenderer(renduPrincipale_survie);
	SDL_DestroyWindow(fenetrePrincipale_survie);

	if(etat_survie != 6 && deuxieme_surface_versus == NULL && deuxieme_texture_versus == NULL){
		Mix_FreeMusic(music_de_fond_survie);
	}

	SDL_DestroyTexture(main_texture_survie);
	SDL_FreeSurface(main_surface_survie);

	if(deuxieme_surface_versus != NULL && deuxieme_texture_versus != NULL){
		if(etat_survie < 12){
			Mix_FreeMusic(music_de_fond_survie);
		}
		SDL_DestroyTexture(deuxieme_texture_versus);
		SDL_FreeSurface(deuxieme_surface_versus);
	}

	TTF_CloseFont(font_general);

	menu(0);
}
/*------------------------------------------*/