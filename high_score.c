#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "survie.h"
#include "menu.h"
#include "high_score.h"

#define TABLEAU_SURVIE 1
#define TABLEAU_VERSUS 2
#define TABLEAU_IA 3
#define QUITTER 4

typedef struct position_struct{
	int x;
	int y;
}position;

int etat_high_score = TABLEAU_SURVIE;

int width_windows_high_score = 0;
int height_windows_high_score = 0;

SDL_Color couleur_font_high_score = {255, 255, 255};

SDL_Window* fenetrePrincipale_high_score = NULL;
SDL_Renderer* renduPrincipale_high_score = NULL;

TTF_Font* font_general_high_score = NULL;

int high_score(){
	reinitialisation_high_score();
	init_high_score();
	get_screensize_high_score();
	set_high_score();
	while(1){
		input_high_score();
		delay_game_high_score();
		//SDL_Log("etat : %d ",etat_high_score);
	}
	return 0;
}

int reinitialisation_high_score(){
	etat_high_score = TABLEAU_SURVIE;
	return 0;
}


int set_high_score(){
	dessin_fond_high_score();
	dessin_fond_high_score();
	dessin_high_score_high_score();
	SDL_RenderPresent(renduPrincipale_high_score);
}


int dessin_texte_high_score(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y){

	SDL_Surface* texte_high_score_surface = TTF_RenderText_Solid(font_general_high_score, texte , couleur);
	SDL_Texture* texte_high_score_texture = SDL_CreateTextureFromSurface(renduPrincipale_high_score, texte_high_score_surface);

	int x_centre = (width_windows_high_score - size_font_x)/2;
	int y_centre = (height_windows_high_score - size_font_y)/2;

	position posi_texte_high_score = {x_centre,y_centre+size_font_y*coef_position};
	
	SDL_Rect texte_high_score = {posi_texte_high_score.x, posi_texte_high_score.y, size_font_x, size_font_y};

	SDL_RenderCopy(renduPrincipale_high_score, texte_high_score_texture, NULL, &texte_high_score);

	SDL_DestroyTexture(texte_high_score_texture);
	SDL_FreeSurface(texte_high_score_surface);
}

int dessin_fond_high_score(){
	int size_high_score = 32;

	SDL_SetRenderDrawColor(renduPrincipale_high_score,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_high_score);
	SDL_SetRenderDrawColor(renduPrincipale_high_score,77, 77, 77, 255);


	for(int x = 0; x <= width_windows_high_score; x += size_high_score){
		SDL_RenderDrawLine(renduPrincipale_high_score, x, 0, x, height_windows_high_score);
	}

	for(int y = 0; y < height_windows_high_score; y += size_high_score){
		SDL_RenderDrawLine(renduPrincipale_high_score, 0, y, width_windows_high_score, y);
	}

	SDL_RenderPresent(renduPrincipale_high_score);
	return 0;
}

int dessin_high_score_high_score(){
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
		read_high_score("./score_IA.txt");
	}
}

int read_high_score(char *file_name){
	FILE *fichier;
	char ligne[128];

	int compteur = -1;

	fichier = fopen(file_name, "a");
	fclose(fichier);

	fichier = fopen(file_name, "r");

	while(fgets(ligne, sizeof(ligne), fichier) != NULL){
		int len = strlen(ligne);
		if(strcmp(&ligne[len-1],"\n") == 0){
			strcpy(&ligne[len-1],"");
		}

		dessin_texte_high_score(ligne,couleur_font_high_score,compteur,400,100);
		compteur++;
	}

	fclose(fichier);

	return 0;
}


int input_high_score(){
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

					}else if(touche.key.keysym.sym == SDLK_ESCAPE){
						switch_screen_high_score();
					}

				break;
			}
		}
	}

int init_high_score(){

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
}

int delay_game_high_score(){
	int lastTicks = 0;
	int maxFPS = 60;
	int delay = 0;

	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

int get_screensize_high_score(){
	SDL_GetWindowSize(fenetrePrincipale_high_score, &width_windows_high_score, &height_windows_high_score);
	return 0;
}

int fermeture_sdl_high_score(){
	SDL_DestroyRenderer(renduPrincipale_high_score);
	SDL_DestroyWindow(fenetrePrincipale_high_score);
	SDL_Quit();
	exit(EXIT_SUCCESS);
	return 0;
}

int switch_screen_high_score(){
	SDL_DestroyRenderer(renduPrincipale_high_score);
	SDL_DestroyWindow(fenetrePrincipale_high_score);
	menu();
	return 0;
}