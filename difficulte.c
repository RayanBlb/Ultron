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

#define FACILE 1
#define NORMAL 2
#define DIFFICILE 3

int etat_difficulte = NORMAL;

int width_windows_difficulte = 0;
int height_windows_difficulte = 0;

SDL_Color couleur_font_difficulte = {255, 255, 255};

SDL_Window* fenetrePrincipale_difficulte = NULL;
SDL_Renderer* renduPrincipale_difficulte = NULL;

TTF_Font* font_general_difficulte = NULL;

//Boucle principale
void difficulte(int mode_difficulte){
	reinitialisation_difficulte(mode_difficulte);
	init_difficulte();
	get_screensize_difficulte();
	set_fond_menu_difficulte();
	while(1){
		input_difficulte();
		delay_game_difficulte();
		//SDL_Log("etat : %d ",etat_difficulte);
	}
}
/*------------------------------------------*/

//Fonction de réinitialisation des variable global
void reinitialisation_difficulte(int mode_difficulte){
	if(mode_difficulte == 1){
		etat_difficulte = FACILE;
	}else if(mode_difficulte == 2){
		etat_difficulte = NORMAL;
	}else if(mode_difficulte == 3){
		etat_difficulte = DIFFICILE;
	}
}
/*------------------------------------------*/

//Fonction d'affichage du menu
void set_difficulte(){
	dessin_menu_difficulte();
	SDL_RenderPresent(renduPrincipale_difficulte);
}

void set_fond_menu_difficulte(){
	dessin_fond_difficulte();
	dessin_texte_difficulte("ULTRON",couleur_font_difficulte,-2,600,175);
	dessin_menu_difficulte();
	SDL_RenderPresent(renduPrincipale_difficulte);
}
/*------------------------------------------*/

//Fonction qui vont permettre de dessiner les différents éléments à afficher
void dessin_texte_difficulte(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y){

	SDL_Surface* texte_difficulte_surface = TTF_RenderText_Solid(font_general_difficulte, texte , couleur);
	SDL_Texture* texte_difficulte_texture = SDL_CreateTextureFromSurface(renduPrincipale_difficulte, texte_difficulte_surface);

	int x_centre = (width_windows_difficulte - size_font_x)/2;
	int y_centre = (height_windows_difficulte - size_font_y)/2;

	position_difficulte posi_texte_difficulte = {x_centre,y_centre+size_font_y*coef_position};
	
	SDL_Rect texte_difficulte = {posi_texte_difficulte.x, posi_texte_difficulte.y, size_font_x, size_font_y};

	SDL_RenderCopy(renduPrincipale_difficulte, texte_difficulte_texture, NULL, &texte_difficulte);

	SDL_DestroyTexture(texte_difficulte_texture);
	SDL_FreeSurface(texte_difficulte_surface);
}

void dessin_fond_difficulte(){
	int size_difficulte = 32;

	SDL_SetRenderDrawColor(renduPrincipale_difficulte,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_difficulte);
	SDL_SetRenderDrawColor(renduPrincipale_difficulte,77, 77, 77, 255);


	for(int x = 0; x <= width_windows_difficulte; x += size_difficulte){
		SDL_RenderDrawLine(renduPrincipale_difficulte, x, 0, x, height_windows_difficulte);
	}

	for(int y = 0; y < height_windows_difficulte; y += size_difficulte){
		SDL_RenderDrawLine(renduPrincipale_difficulte, 0, y, width_windows_difficulte, y);
	}
}

void dessin_menu_difficulte(){
	SDL_Color couleur_font_selection = {255, 0, 0};

	if(etat_difficulte == FACILE){
		dessin_texte_difficulte("Facile",couleur_font_selection,-2,400,100);
		dessin_texte_difficulte("Normal",couleur_font_difficulte,-1,400,100);
		dessin_texte_difficulte("Difficile",couleur_font_difficulte,0,400,100);
	}else if(etat_difficulte == NORMAL ){
		dessin_texte_difficulte("Facile",couleur_font_difficulte,-2,400,100);
		dessin_texte_difficulte("Normal",couleur_font_selection,-1,400,100);
		dessin_texte_difficulte("Difficile",couleur_font_difficulte,0,400,100);
	}else if(etat_difficulte == DIFFICILE ){
		dessin_texte_difficulte("Facile",couleur_font_difficulte,-2,400,100);
		dessin_texte_difficulte("Normal",couleur_font_difficulte,-1,400,100);
		dessin_texte_difficulte("Difficile",couleur_font_selection,0,400,100);
	}
}
/*------------------------------------------*/

//Fonction relative au input du joueur
void input_difficulte(){
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_difficulte();
				break;
			case SDL_QUIT:
				fermeture_sdl_difficulte();
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_UP ){
					if(etat_difficulte > FACILE){
						etat_difficulte--;
					}
					set_difficulte();

				}else if(touche.key.keysym.sym == SDLK_DOWN ){
					if(etat_difficulte < DIFFICILE){
						etat_difficulte++;
					}
					set_difficulte();

				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_difficulte(0);

				}else if(touche.key.keysym.sym == SDLK_RETURN){
					if (etat_difficulte == FACILE){
						switch_screen_difficulte(1);
					}else if(etat_difficulte == NORMAL){
						switch_screen_difficulte(2);
					}else if(etat_difficulte == DIFFICILE){
						switch_screen_difficulte(3);
					}
				}
				break;
			}
		}
	}
/*------------------------------------------*/

//Fonction initialisation
int init_difficulte(){

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

	if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_SHOWN, &fenetrePrincipale_difficulte, &renduPrincipale_difficulte) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_difficulte);
    SDL_RenderPresent(renduPrincipale_difficulte);

    SDL_SetWindowTitle(fenetrePrincipale_difficulte, "Ultron");

    SDL_Surface* icon_programme_survie = SDL_LoadBMP("./Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_difficulte, icon_programme_survie);

    font_general_difficulte = TTF_OpenFont("./Font/font.ttf", 16);

    return 0;
}
/*------------------------------------------*/

//Fonction qui vont permettre de gérer le delay, la fermetture et changement de fenetre ou encore de récuperer la taille de la fenetre afficher
void delay_game_difficulte(){
	int lastTicks = 0;
	int maxFPS = 60;
	int delay = 0;

	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

void get_screensize_difficulte(){
	SDL_GetWindowSize(fenetrePrincipale_difficulte, &width_windows_difficulte, &height_windows_difficulte);
}

void fermeture_sdl_difficulte(){
	SDL_DestroyRenderer(renduPrincipale_difficulte);
	SDL_DestroyWindow(fenetrePrincipale_difficulte);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void switch_screen_difficulte(int mode_difficulte){
	SDL_DestroyRenderer(renduPrincipale_difficulte);
	SDL_DestroyWindow(fenetrePrincipale_difficulte);
	menu(mode_difficulte);
}
/*------------------------------------------*/