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

#define TABLEAU_SURVIE 1
#define TABLEAU_VERSUS 2
#define TABLEAU_IA 3
#define QUITTER 4


int etat_high_score = TABLEAU_SURVIE; //Etat du menu high score

int width_windows_high_score = 0;
int height_windows_high_score = 0;

SDL_Color couleur_font_high_score = {255, 255, 255};

SDL_Window* fenetrePrincipale_high_score = NULL;
SDL_Renderer* renduPrincipale_high_score = NULL;

TTF_Font* font_general_high_score = NULL;

//Boucle principale
void high_score(){
	reinitialisation_high_score();//Réinitialisation des variables globales
	init_high_score();//Initialisation de SDL et des autre bibliothéque 
	get_screensize_high_score();//Récuperation de la taille de la fenetre
	set_high_score();//Affichage du High score + fond + titre
	while(1){
		input_high_score();//Changement d'état du menu en fonction des input
		delay_game_high_score();//Taux de rafraîchissement de 60 fps
		//SDL_Log("etat : %d ",etat_high_score);
	}
}
/*------------------------------------------*/

//Fonction de réinitialisation des variable global
void reinitialisation_high_score(){
	etat_high_score = TABLEAU_SURVIE;
}
/*------------------------------------------*/

//Fonction d'affichage du high score
void set_high_score(){
	dessin_fond_high_score();//création fond quadrillé
	dessin_high_score_high_score();//Affichage du classement en fonction du mode de jeu
	SDL_RenderPresent(renduPrincipale_high_score);//Actualisation de la fenetre
}
/*------------------------------------------*/

//Fonction qui vont permettre de dessiner les différents éléments à afficher
void dessin_texte_high_score(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y){/*Fonction qui prend en argument un texte a écrire, la couleur de ce texte, la taille du texte ainsi que la position x et y
permet donc de facilité l'affichage de texte*/
	SDL_Surface* texte_high_score_surface = TTF_RenderText_Solid(font_general_high_score, texte , couleur);
	SDL_Texture* texte_high_score_texture = SDL_CreateTextureFromSurface(renduPrincipale_high_score, texte_high_score_surface);

	int x_centre = (width_windows_high_score - size_font_x)/2;
	int y_centre = (height_windows_high_score - size_font_y)/2;

	position_high_score posi_texte_high_score = {x_centre,y_centre+size_font_y*coef_position};
	
	SDL_Rect texte_high_score = {posi_texte_high_score.x, posi_texte_high_score.y, size_font_x, size_font_y};

	SDL_RenderCopy(renduPrincipale_high_score, texte_high_score_texture, NULL, &texte_high_score);

	SDL_DestroyTexture(texte_high_score_texture);
	SDL_FreeSurface(texte_high_score_surface);
}

void dessin_fond_high_score(){//création du fond quadrillé
	int size_high_score = 32;//Correspond a la taille des carreaux du quadrillage

	SDL_SetRenderDrawColor(renduPrincipale_high_score,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_high_score);
	SDL_SetRenderDrawColor(renduPrincipale_high_score,77, 77, 77, 255);


	for(int x = 0; x <= width_windows_high_score; x += size_high_score){
		SDL_RenderDrawLine(renduPrincipale_high_score, x, 0, x, height_windows_high_score);
	}

	for(int y = 0; y < height_windows_high_score; y += size_high_score){
		SDL_RenderDrawLine(renduPrincipale_high_score, 0, y, width_windows_high_score, y);
	}
}

void dessin_high_score_high_score(){//Affichage du high score en fonction du mode de jeu selectionner
	if(etat_high_score == TABLEAU_SURVIE){
		dessin_texte_high_score("HIGH SCORE",couleur_font_high_score,-2,600,175);
		dessin_texte_high_score("Mode : Survie",couleur_font_high_score,-2,400,110);
		read_high_score("./score_survie.txt");
	}else if(etat_high_score == TABLEAU_VERSUS){
		dessin_texte_high_score("HIGH SCORE",couleur_font_high_score,-2,600,175);
		dessin_texte_high_score("Mode : Versus",couleur_font_high_score,-2,400,110);
		read_high_score("./score_versus.txt");
	}else if(etat_high_score == TABLEAU_IA){
		dessin_texte_high_score("HIGH SCORE",couleur_font_high_score,-2,600,175);
		dessin_texte_high_score("Mode : IA",couleur_font_high_score,-2,400,110);
		read_high_score("./score_ia.txt");
	}
}

void read_high_score(char *file_name){ //Permet de lire le fichier high score afin de l'afficher
	FILE *fichier;
	char ligne[128];

	int compteur = -1;

	fichier = fopen(file_name, "a");//créer un fichier dans le cas ou le fichier n'est pas créer
	fclose(fichier);

	fichier = fopen(file_name, "r");

	while(fgets(ligne, sizeof(ligne), fichier) != NULL){
		int len = strlen(ligne);

		if(strcmp(&ligne[len-1],"\n") == 0){//Permet de retier le caracter saut de ligne
			strcpy(&ligne[len-1],"");
		}

		dessin_texte_high_score(ligne,couleur_font_high_score,compteur,400,100);//Permet d'afficher la ligne du fichier
		compteur++;
	}

	fclose(fichier);
}
/*------------------------------------------*/

//Fonction qui vont permettre de supprimer le contenu d'un fichier
void supprimer_classement_high_score(){//Permet de supprimer le high score d'un mode
	if(etat_high_score == TABLEAU_SURVIE){
		write_high_score("./score_survie.txt");
	}else if(etat_high_score == TABLEAU_VERSUS){
		write_high_score("./score_versus.txt");
	}else if(etat_high_score == TABLEAU_IA){
		write_high_score("./score_ia.txt");
	}
}

void write_high_score(char *file_name){//Permet de vider un fichier
	FILE *fichier;
	fichier = fopen(file_name, "w");
	fclose(fichier);
}
/*------------------------------------------*/

//Fonction relative au input du joueur
void input_high_score(){//permet de changer de high score ainsi que supprimer le high score avec le clavier
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_high_score();
				break;
			case SDL_QUIT:
				fermeture_sdl_high_score();
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_LEFT){

					if(etat_high_score > TABLEAU_SURVIE){
						etat_high_score--;
					}
					set_high_score();

					}else if (touche.key.keysym.sym == SDLK_RIGHT){

					if(etat_high_score < TABLEAU_IA){
						etat_high_score++;
					}
					set_high_score();

					}else if(touche.key.keysym.sym == SDLK_s){
						supprimer_classement_high_score();
						set_high_score();
					}else if(touche.key.keysym.sym == SDLK_ESCAPE){
						switch_screen_high_score();
					}

				break;
			}
		}
	}
/*------------------------------------------*/

//Fonction initialisation
int init_high_score(){//Fonction qui initialise SDL ainsi que c'est bibliothéque, donne un titre a la fenetre ainsi qu'un icon, viens initialisé la font

	if(SDL_Init(SDL_INIT_VIDEO < 0)){
		printf("Erreur d'initialisation de SDL VIDEO: %s",SDL_GetError());
		return EXIT_FAILURE;
	}

	if(SDL_Init(SDL_INIT_AUDIO) < 0){
		printf("Erreur d'initialisation de SDL AUDIO : %s",SDL_GetError());
		return EXIT_FAILURE;
	}

	atexit(SDL_Quit);

	if(TTF_Init() < 0){
		printf("TTF_Init: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}

	if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
		printf("Audio mix : %s", Mix_GetError());
	}

	if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_SHOWN, &fenetrePrincipale_high_score, &renduPrincipale_high_score) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_high_score);
    SDL_RenderPresent(renduPrincipale_high_score);

    SDL_SetWindowTitle(fenetrePrincipale_high_score, "Ultron");

    SDL_Surface* icon_programme_survie = SDL_LoadBMP("./Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_high_score, icon_programme_survie);

    font_general_high_score = TTF_OpenFont("./Font/font.ttf", 16);

    return 0;
}
/*------------------------------------------*/

//Fonction qui vont permettre de gérer le delay, la fermetture et changement de fenetre ou encore de récuperer la taille de la fenetre afficher
void delay_game_high_score(){//Permet de gerer le taux de réfréchissement de l'écran
	int lastTicks = 0;
	int maxFPS = 60;
	int delay = 0;

	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

void get_screensize_high_score(){//Récuperer la taille de la fenetre
	SDL_GetWindowSize(fenetrePrincipale_high_score, &width_windows_high_score, &height_windows_high_score);
}

void fermeture_sdl_high_score(){//Permet de fermet la fenetre
	SDL_DestroyRenderer(renduPrincipale_high_score);
	SDL_DestroyWindow(fenetrePrincipale_high_score);
	TTF_CloseFont(font_general_high_score);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void switch_screen_high_score(){//Permet de revenir au menu principale
	SDL_DestroyRenderer(renduPrincipale_high_score);
	SDL_DestroyWindow(fenetrePrincipale_high_score);
	TTF_CloseFont(font_general_high_score);
	menu(0);
}
/*------------------------------------------*/