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

#define CLASSIQUE 1
#define DE_HAUTE_EN_BAS 2
#define MIRROIR 3

int etat_mode_ia = CLASSIQUE;

int width_windows_mode_ia = 0;
int height_windows_mode_ia = 0;

SDL_Color couleur_font_mode_ia = {255, 255, 255};

SDL_Window* fenetrePrincipale_mode_ia = NULL;
SDL_Renderer* renduPrincipale_mode_ia = NULL;

TTF_Font* font_general_mode_ia = NULL;

//Boucle principale
void mode_ia(int mode_difficulte){
	reinitialisation_mode_ia(mode_difficulte);
	init_mode_ia();
	get_screensize_mode_ia();
	set_fond_menu_mode_ia();
	while(1){
		input_mode_ia(mode_difficulte);
		delay_game_mode_ia();
		//SDL_Log("etat : %d ",etat_mode_ia);
	}
}
/*------------------------------------------*/

//Fonction de réinitialisation des variable global
void reinitialisation_mode_ia(int mode_difficulte){
	etat_mode_ia = CLASSIQUE;
}
/*------------------------------------------*/

//Fonction d'affichage du menu du choix de l'ia
void set_mode_ia(){
	dessin_menu_mode_ia();
	SDL_RenderPresent(renduPrincipale_mode_ia);
}

void set_fond_menu_mode_ia(){
	dessin_fond_mode_ia();
	dessin_texte_mode_ia("ULTRON",couleur_font_mode_ia,-2,600,175);
	dessin_menu_mode_ia();
	SDL_RenderPresent(renduPrincipale_mode_ia);
}
/*------------------------------------------*/

//Fonction qui vont permettre de dessiner les différents éléments à afficher
void dessin_texte_mode_ia(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y){

	SDL_Surface* texte_mode_ia_surface = TTF_RenderText_Solid(font_general_mode_ia, texte , couleur);
	SDL_Texture* texte_mode_ia_texture = SDL_CreateTextureFromSurface(renduPrincipale_mode_ia, texte_mode_ia_surface);

	int x_centre = (width_windows_mode_ia - size_font_x)/2;
	int y_centre = (height_windows_mode_ia - size_font_y)/2;

	position_mode_ia posi_texte_mode_ia = {x_centre,y_centre+size_font_y*coef_position};
	
	SDL_Rect texte_mode_ia = {posi_texte_mode_ia.x, posi_texte_mode_ia.y, size_font_x, size_font_y};

	SDL_RenderCopy(renduPrincipale_mode_ia, texte_mode_ia_texture, NULL, &texte_mode_ia);

	SDL_DestroyTexture(texte_mode_ia_texture);
	SDL_FreeSurface(texte_mode_ia_surface);
}

void dessin_fond_mode_ia(){
	int size_mode_ia = 32;

	SDL_SetRenderDrawColor(renduPrincipale_mode_ia,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_mode_ia);
	SDL_SetRenderDrawColor(renduPrincipale_mode_ia,77, 77, 77, 255);


	for(int x = 0; x <= width_windows_mode_ia; x += size_mode_ia){
		SDL_RenderDrawLine(renduPrincipale_mode_ia, x, 0, x, height_windows_mode_ia);
	}

	for(int y = 0; y < height_windows_mode_ia; y += size_mode_ia){
		SDL_RenderDrawLine(renduPrincipale_mode_ia, 0, y, width_windows_mode_ia, y);
	}
}

void dessin_menu_mode_ia(){
	SDL_Color couleur_font_selection = {255, 0, 0};

	if(etat_mode_ia == CLASSIQUE){
		dessin_texte_mode_ia("CLASSIQUE",couleur_font_selection,-1,400,100);
		dessin_texte_mode_ia("DE HAUTE EN BAS",couleur_font_mode_ia,0,400,100);
		dessin_texte_mode_ia("MIROIR",couleur_font_mode_ia,1,400,100);
	}else if(etat_mode_ia == DE_HAUTE_EN_BAS ){
		dessin_texte_mode_ia("CLASSIQUE",couleur_font_mode_ia,-1,400,100);
		dessin_texte_mode_ia("DE HAUTE EN BAS",couleur_font_selection,0,400,100);
		dessin_texte_mode_ia("MIROIR",couleur_font_mode_ia,1,400,100);
	}else if(etat_mode_ia == MIRROIR ){
		dessin_texte_mode_ia("CLASSIQUE",couleur_font_mode_ia,-1,400,100);
		dessin_texte_mode_ia("DE HAUTE EN BAS",couleur_font_mode_ia,0,400,100);
		dessin_texte_mode_ia("MIROIR",couleur_font_selection,1,400,100);
	}
}
/*------------------------------------------*/

//Fonction relative au input du joueur
void input_mode_ia(int mode_difficulte){
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_mode_ia(mode_difficulte);
				break;
			case SDL_QUIT:
				fermeture_sdl_mode_ia();
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_UP ){
					if(etat_mode_ia > CLASSIQUE){
						etat_mode_ia--;
					}
					set_mode_ia();

				}else if(touche.key.keysym.sym == SDLK_DOWN ){
					if(etat_mode_ia < MIRROIR){
						etat_mode_ia++;
					}
					set_mode_ia();

				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_mode_ia(0);

				}else if(touche.key.keysym.sym == SDLK_RETURN){
					if (etat_mode_ia == CLASSIQUE){
						switch_screen_type_ia(mode_difficulte,1);
					}else if(etat_mode_ia == DE_HAUTE_EN_BAS){
						switch_screen_type_ia(mode_difficulte,2);
					}else if(etat_mode_ia == MIRROIR){
						switch_screen_type_ia(mode_difficulte,3);
					}
				}
				break;
			}
		}
	}
/*------------------------------------------*/

//Fonction initialisation
int init_mode_ia(){

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

	if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_SHOWN, &fenetrePrincipale_mode_ia, &renduPrincipale_mode_ia) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_mode_ia);
    SDL_RenderPresent(renduPrincipale_mode_ia);

    SDL_SetWindowTitle(fenetrePrincipale_mode_ia, "Ultron");

    SDL_Surface* icon_programme_survie = SDL_LoadBMP("./Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_mode_ia, icon_programme_survie);

    font_general_mode_ia = TTF_OpenFont("./Font/font.ttf", 16);
    
    return 0;
}
/*------------------------------------------*/

//Fonction qui vont permettre de gérer le delay, la fermetture et changement de fenetre ou encore de récuperer la taille de la fenetre afficher
void delay_game_mode_ia(){
	int lastTicks = 0;
	int maxFPS = 60;
	int delay = 0;

	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

void get_screensize_mode_ia(){
	SDL_GetWindowSize(fenetrePrincipale_mode_ia, &width_windows_mode_ia, &height_windows_mode_ia);
}

void fermeture_sdl_mode_ia(){
	SDL_DestroyRenderer(renduPrincipale_mode_ia);
	SDL_DestroyWindow(fenetrePrincipale_mode_ia);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void switch_screen_type_ia(int mode_difficulte,int type_ia){
	SDL_DestroyRenderer(renduPrincipale_mode_ia);
	SDL_DestroyWindow(fenetrePrincipale_mode_ia);
	ia(mode_difficulte,type_ia);
}

void switch_screen_mode_ia(int mode_difficulte){
	SDL_DestroyRenderer(renduPrincipale_mode_ia);
	SDL_DestroyWindow(fenetrePrincipale_mode_ia);
	menu(mode_difficulte);
}
/*------------------------------------------*/