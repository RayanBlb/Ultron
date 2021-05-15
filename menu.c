#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "survie.h"
#include "menu.h"

#define START_SURVIE 1
#define OPTION 2
#define HIGH_SCORE 3
#define QUITTER 4

typedef struct position_struct{
	int x;
	int y;
}position;

int etat_menu = START_SURVIE;

int width_windows_menu = 0;
int height_windows_menu = 0;

SDL_Color couleur_font_menu = {255, 255, 255};

SDL_Window* fenetrePrincipale_menu = NULL;
SDL_Renderer* renduPrincipale_menu = NULL;

TTF_Font* font_general_menu = NULL;

int menu(){
	reinitialisation_menu();
	init_menu();
	get_screensize_menu();
	set_fond_menu_menu();
	while(1){
		input_menu();
		delay_game_menu();
		SDL_Log("etat : %d ",etat_menu);
	}
	return 0;
}

int reinitialisation_menu(){
	etat_menu = START_SURVIE;
	return 0;
}

int set_menu(){
	dessin_menu_menu();
	SDL_RenderPresent(renduPrincipale_menu);
}

int set_fond_menu_menu(){
	dessin_fond_menu();
	dessin_texte_menu("ULTRON",couleur_font_menu,-2,600,175);
	dessin_menu_menu();
	SDL_RenderPresent(renduPrincipale_menu);
}


int dessin_texte_menu(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y){

	SDL_Surface* texte_menu_surface = TTF_RenderText_Solid(font_general_menu, texte , couleur);
	SDL_Texture* texte_menu_texture = SDL_CreateTextureFromSurface(renduPrincipale_menu, texte_menu_surface);

	int x_centre = (width_windows_menu - size_font_x)/2;
	int y_centre = (height_windows_menu - size_font_y)/2;

	position posi_texte_menu = {x_centre,y_centre+size_font_y*coef_position};
	
	SDL_Rect texte_menu = {posi_texte_menu.x, posi_texte_menu.y, size_font_x, size_font_y};

	SDL_RenderCopy(renduPrincipale_menu, texte_menu_texture, NULL, &texte_menu);

	SDL_DestroyTexture(texte_menu_texture);
	SDL_FreeSurface(texte_menu_surface);
}

int dessin_fond_menu(){
	int size_menu = 32;

	SDL_SetRenderDrawColor(renduPrincipale_menu,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_menu);
	SDL_SetRenderDrawColor(renduPrincipale_menu,77, 77, 77, 255);


	for(int x = 0; x <= width_windows_menu; x += size_menu){
		SDL_RenderDrawLine(renduPrincipale_menu, x, 0, x, height_windows_menu);
	}

	for(int y = 0; y < height_windows_menu; y += size_menu){
		SDL_RenderDrawLine(renduPrincipale_menu, 0, y, width_windows_menu, y);
	}

	SDL_RenderPresent(renduPrincipale_menu);
	return 0;
}

int dessin_menu_menu(){
	SDL_Color couleur_font_selection = {255, 0, 0};

	if(etat_menu == START_SURVIE){
		dessin_texte_menu("Jouer !",couleur_font_selection,-1,400,125);
		dessin_texte_menu("Option",couleur_font_menu,0,400,125);
		dessin_texte_menu("High Score",couleur_font_menu,1,400,125);
		dessin_texte_menu("Quitter",couleur_font_menu,2,400,125);
	}else if(etat_menu == OPTION ){
		dessin_texte_menu("Jouer !",couleur_font_menu,-1,400,125);
		dessin_texte_menu("Option",couleur_font_selection,0,400,125);
		dessin_texte_menu("High Score",couleur_font_menu,1,400,125);
		dessin_texte_menu("Quitter",couleur_font_menu,2,400,125);
	}else if(etat_menu == HIGH_SCORE ){
		dessin_texte_menu("Jouer !",couleur_font_menu,-1,400,125);
		dessin_texte_menu("Option",couleur_font_menu,0,400,125);
		dessin_texte_menu("High Score",couleur_font_selection,1,400,125);
		dessin_texte_menu("Quitter",couleur_font_menu,2,400,125);
	}else if(etat_menu == QUITTER ){
		dessin_texte_menu("Jouer !",couleur_font_menu,-1,400,125);
		dessin_texte_menu("Option",couleur_font_menu,0,400,125);
		dessin_texte_menu("High Score",couleur_font_menu,1,400,125);
		dessin_texte_menu("Quitter",couleur_font_selection,2,400,125);	
	}
}

int input_menu(){
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_menu();
				break;
			case SDL_QUIT:
				fermeture_sdl_menu();
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_UP ){
					if(etat_menu > START_SURVIE){
						etat_menu--;
					}
					set_menu();

				}else if(touche.key.keysym.sym == SDLK_DOWN ){
					if(etat_menu < QUITTER){
						etat_menu++;
					}
					set_menu();

				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					fermeture_sdl_menu();

				}else if(touche.key.keysym.sym == SDLK_RETURN){
					if (etat_menu == START_SURVIE){
						switch_screen_menu();
					}else if(etat_menu == OPTION){

					}else if(etat_menu == HIGH_SCORE){
						
					}else if(etat_menu == QUITTER){
						fermeture_sdl_menu();
					}
				}
				break;
			}
		}
	}

int init_menu(){

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

	if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_SHOWN, &fenetrePrincipale_menu, &renduPrincipale_menu) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_menu);
    SDL_RenderPresent(renduPrincipale_menu);

    SDL_SetWindowTitle(fenetrePrincipale_menu, "Ultron");

    SDL_Surface* icon_programme_survie = SDL_LoadBMP("./Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_menu, icon_programme_survie);

    font_general_menu = TTF_OpenFont("./Font/font.ttf", 16);
}

int delay_game_menu(){
	int lastTicks = 0;
	int maxFPS = 60;
	int delay = 0;

	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

int get_screensize_menu(){
	SDL_GetWindowSize(fenetrePrincipale_menu, &width_windows_menu, &height_windows_menu);
	return 0;
}

int fermeture_sdl_menu(){
	SDL_DestroyRenderer(renduPrincipale_menu);
	SDL_DestroyWindow(fenetrePrincipale_menu);
	SDL_Quit();
	exit(EXIT_SUCCESS);
	return 0;
}

int switch_screen_menu(){
	SDL_DestroyRenderer(renduPrincipale_menu);
	SDL_DestroyWindow(fenetrePrincipale_menu);
	survie();
	return 0;
}