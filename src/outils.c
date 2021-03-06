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
void read_file_high_score_outils(char *chemin,char *nom_high_score_survie,int score_survie,liste *pFirst){//Permet d'ajouter un nouveau score dans les high score
	FILE *fichier;
	char ligne[128];

	fichier = fopen(chemin, "a");
	fclose(fichier);

	fichier = fopen(chemin, "r");

	if(strcmp(&nom_high_score_survie[0],"") != 0){//Vérifie si le joueur a bien rentrer un nom
		tri_high_score_outils(&pFirst,nom_high_score_survie,score_survie);//Ajoute le joueur dans la liste
	}else if(strcmp(&nom_high_score_survie[0],"") == 0){
		tri_high_score_outils(&pFirst,"Joueur inconnu",score_survie);
	}

	while(fgets(ligne, sizeof(ligne), fichier) != NULL){//Récupere les autres high score du fichier txt et les met dans la liste

		char *nom = strtok(ligne,":");
		int score = strtol(strtok(NULL,":"),NULL,10);

		tri_high_score_outils(&pFirst,nom,score);
	}

	fclose(fichier);

	sup_liste_high_score_outils(chemin);//Supprime le contenu du fichier avec l'ancienne liste
	write_liste_high_score_outils(pFirst,chemin);//Réécris la nouvelle liste
	free_liste_high_score_outils(&pFirst);//free la liste
}

int tri_high_score_outils(liste **pFirst,char *nom,int score){//algo de tri afin de trier la liste du plus grand au plus petit
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

void write_liste_high_score_outils(liste *pFirst,char *chemin){//Viens écrire ma liste dans un fichier
	liste *pTemp = pFirst;
	int compteur = 0;

	while(pTemp != NULL && compteur <= 4){
		write_file_outils(pTemp->nom,pTemp->score,chemin);
		compteur++;
		pTemp=pTemp->pNext;
	}
}

void sup_liste_high_score_outils(char *chemin){//Permet de vider le contenu d'un fichier high score
	FILE *fichier;
	fichier = fopen (chemin,"w");
	fclose(fichier);
}

void write_file_outils(char *nom, int score,char *chemin){//permet d'ecrire dans mes fichier high score
	FILE *fichier;
	fichier = fopen (chemin,"a");
	fprintf (fichier,"%s:%d \n",nom,score);
	fclose(fichier);
}

void free_liste_high_score_outils(liste **pFirst){//Permet de free ma liste
	liste *temp_un = *pFirst;
	liste *temp_deux = NULL;

	while(temp_un != NULL){
		temp_deux = temp_un->pNext;
		free(temp_un->nom);
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
		if(etat_survie == 6) maxFPS_survie = 60;//Passe en mode 60 fps pour ecrire son nom de high score
	}else if(mode_de_jeu == 1){
		if(etat_survie != 1) maxFPS_survie = 15; //Enleve le petit décalage de lancement
		if(etat_survie >= 12) maxFPS_survie = 60;//Passe en mode 60 fps pour ecrire son nom de high score
	}

	lastTicks_survie = SDL_GetTicks();
	delay_survie = 1000/maxFPS_survie-SDL_GetTicks()+lastTicks_survie;
	if(delay_survie>0){
		SDL_Delay(delay_survie);
	}
}
/*------------------------------------------*/

//Gestion de la musique
void play_musique_outils(Mix_Music **music_de_fond_survie){//Viens jouer la musique
	*music_de_fond_survie = Mix_LoadMUS("../Musique/music.mp3");
	Mix_PlayMusic(*music_de_fond_survie, -1);
}

void play_explosion_outils(Mix_Chunk **explosion_survie){//Viens jouer le bruit d'explosion
	*explosion_survie = Mix_LoadWAV("../Musique/explosion.wav");
	Mix_PlayChannel(-1, *explosion_survie, 0);
}
/*------------------------------------------*/

//Gestion du tableau allocation memoire
void allocation_tableau_outils(int ***tableau_deplacement, int width_windows_survie, int height_windows_survie){//Viens allouer la memoire nécessaire afin de créer un tableau et viens renplir tout le tableau de 0
  
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

void free_tableau_outils(int ***tableau_deplacement, int width_windows_survie){//Viens free le tableau
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
//ces fonction viennent détruire les différents élèments créer en fonction du mode
void fermeture_sdl_outils(SDL_Window *fenetrePrincipale_survie, SDL_Renderer *renduPrincipale_survie, Mix_Chunk *explosion_survie, SDL_Surface *main_surface_survie, SDL_Texture *main_texture_survie,SDL_Surface *deuxieme_surface_versus,SDL_Texture *deuxieme_texture_versus,TTF_Font* font_general){
	SDL_DestroyRenderer(renduPrincipale_survie);
	SDL_DestroyWindow(fenetrePrincipale_survie);

	Mix_FreeChunk(explosion_survie);

	SDL_DestroyTexture(main_texture_survie);
	SDL_FreeSurface(main_surface_survie);

	if(deuxieme_surface_versus != NULL && deuxieme_texture_versus != NULL){//dans le cas ou il y a un deuxieme joueur a detruire
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
		Mix_FreeMusic(music_de_fond_survie);//Dans le cas ou il y a encore de la musique jouer car pas de game over (le game over arrete la musique)
	}

	SDL_DestroyTexture(main_texture_survie);
	SDL_FreeSurface(main_surface_survie);

	if(deuxieme_surface_versus != NULL && deuxieme_texture_versus != NULL){
		if(etat_survie < 12){
			Mix_FreeMusic(music_de_fond_survie);//Dans le cas ou il y a encore de la musique jouer car pas de game over (le game over arrete la musique)
		}
		SDL_DestroyTexture(deuxieme_texture_versus);//dans le cas ou il y a un deuxieme joueur a detruire
		SDL_FreeSurface(deuxieme_surface_versus);
	}

	TTF_CloseFont(font_general);

	menu(0);
}
/*------------------------------------------*/

//dessin personnage via liste
int log_deplacement_outils(listePosition **pFirst,int position_x,int position_y){
	listePosition *a_ajouter = (listePosition *)malloc(sizeof(listePosition));

	listePosition *temp = *pFirst;

	a_ajouter->x = position_x;
	a_ajouter->y = position_y;
	a_ajouter->pNext = NULL;

	if(pFirst == NULL)return 0;

	if(*pFirst == NULL){
		*pFirst = a_ajouter;
		return 0;
	}

	while(temp->pNext != NULL){
		temp = temp->pNext;
	}

	temp->pNext = a_ajouter;

	return 0;
}

void dessin_personnage_outils(listePosition *pFirst,SDL_Renderer *renduPrincipale,SDL_Texture *main_texture,int size_main){//création du premier joueurs (joueur blanc)
	listePosition *pTemp = pFirst;

	while(pTemp != NULL){
		SDL_Rect dest = {pTemp->x, pTemp->y, size_main, size_main};
		SDL_RenderCopy(renduPrincipale,main_texture,NULL,&dest);
		pTemp=pTemp->pNext;
	}
}

void free_log_deplacement_outils(listePosition **pFirst){//Permet de free ma liste
	listePosition *temp_un = *pFirst;
	listePosition *temp_deux = NULL;

	while(temp_un != NULL){
		temp_deux = temp_un->pNext;
		free(temp_un);
		temp_un = temp_deux;
	}

	*pFirst = NULL;
}
/*------------------------------------------*/

//Fonction d'affichage commune a plusieurs fichier

void dessin_texte_outils(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y,TTF_Font *font_general,int width_windows, int height_windows,SDL_Renderer *renduPrincipale){ /*Fonction qui prend en argument un texte a écrire, la couleur de ce texte, la taille du texte ainsi que la position x et y
permet donc de facilité l'affichage de texte*/
	SDL_Surface* texte_surface = TTF_RenderText_Solid(font_general, texte , couleur);
	SDL_Texture* texte_texture = SDL_CreateTextureFromSurface(renduPrincipale, texte_surface);

	int x_centre = (width_windows - size_font_x)/2;
	int y_centre = (height_windows - size_font_y)/2;

	position posi_texte= {x_centre,y_centre+size_font_y*coef_position};
	
	SDL_Rect texte_forme = {posi_texte.x, posi_texte.y, size_font_x, size_font_y};

	SDL_RenderCopy(renduPrincipale, texte_texture, NULL, &texte_forme);

	SDL_DestroyTexture(texte_texture);
	SDL_FreeSurface(texte_surface);
}

void dessin_fond_outils(int taille_carreaux,SDL_Renderer *renduPrincipale,int width_windows, int height_windows){ //création du fond quadrillé

	SDL_SetRenderDrawColor(renduPrincipale,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale);
	SDL_SetRenderDrawColor(renduPrincipale,77, 77, 77, 255);


	for(int x = 0; x <= width_windows; x += taille_carreaux){
		SDL_RenderDrawLine(renduPrincipale, x, 0, x, height_windows);
	}

	for(int y = 0; y < height_windows; y += taille_carreaux){
		SDL_RenderDrawLine(renduPrincipale, 0, y, width_windows, y);
	}
}