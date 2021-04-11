#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "survie.h"

typedef struct position_struct{
	int x;
	int y;
}position;

int etat_main = 1;
int size_main = 32;

int width_windows_main = 0;
int height_windows_main = 0;

SDL_Event touche_main;

SDL_Color couleur_font_main = {255, 255, 255};

SDL_Window* fenetrePrincipale_main = NULL;
SDL_Renderer* renduPrincipale_main = NULL;

TTF_Font* font_general_main = NULL;

int init_main(void);
int input_main(void);
int set_main(void);
int delay_game_main(void);
int get_screensize_main(void);
int fermeture_sdl_main(void);
int dessin_texte_main(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);

int main(int argc, char *argv[]){
	init_main();
	get_screensize_main();
	set_main();
	while(1){
		input_main();
		set_main();
		delay_game_main();
		SDL_Log("1 - debug : etat_main : %d ",etat_main);
	}
	return 0;
}

int dessin_menu(){
	SDL_Color couleur_font_selection = {255, 0, 0};
	if(etat_main == 1){
		dessin_texte_main("Start Survie",couleur_font_selection,-1,400,125);
		dessin_texte_main("Option",couleur_font_main,0,400,125);
		dessin_texte_main("High Score",couleur_font_main,1,400,125);
		dessin_texte_main("Quitter",couleur_font_main,2,400,125);

	}else if(etat_main == 2 ){
		dessin_texte_main("Start Survie",couleur_font_main,-1,400,125);
		dessin_texte_main("Option",couleur_font_selection,0,400,125);
		dessin_texte_main("High Score",couleur_font_main,1,400,125);
		dessin_texte_main("Quitter",couleur_font_main,2,400,125);

	}else if(etat_main == 3 ){
		dessin_texte_main("Start Survie",couleur_font_main,-1,400,125);
		dessin_texte_main("Option",couleur_font_main,0,400,125);
		dessin_texte_main("High Score",couleur_font_selection,1,400,125);
		dessin_texte_main("Quitter",couleur_font_main,2,400,125);
		
	}else if(etat_main == 4 ){
		dessin_texte_main("Start Survie",couleur_font_main,-1,400,125);
		dessin_texte_main("Option",couleur_font_main,0,400,125);
		dessin_texte_main("High Score",couleur_font_main,1,400,125);
		dessin_texte_main("Quitter",couleur_font_selection,2,400,125);	
	}
}

int dessin_texte_main(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y){

	SDL_Surface* play_survie_surface = TTF_RenderText_Solid(font_general_main, texte , couleur);
	SDL_Texture* play_survie_texture = SDL_CreateTextureFromSurface(renduPrincipale_main, play_survie_surface);

	int x_centre = (width_windows_main - size_font_x)/2;
	int y_centre = (height_windows_main - size_font_y)/2;

	position posi_play_survie = {x_centre,y_centre+size_font_y*coef_position};
	
	SDL_Rect play_survie = {posi_play_survie.x, posi_play_survie.y, size_font_x, size_font_y};

	SDL_RenderCopy(renduPrincipale_main, play_survie_texture, NULL, &play_survie);
}

int dessin_fond(){
	SDL_SetRenderDrawColor(renduPrincipale_main,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_main);
	SDL_SetRenderDrawColor(renduPrincipale_main,77, 77, 77, 255);


	for(int x = 0; x <= width_windows_main; x += size_main){
		SDL_RenderDrawLine(renduPrincipale_main, x, 0, x, height_windows_main);
	}

	for(int y = 0; y < height_windows_main; y += size_main){
		SDL_RenderDrawLine(renduPrincipale_main, 0, y, width_windows_main, y);
	}

	SDL_RenderPresent(renduPrincipale_main);
	return 0;
}

int set_main(){
	dessin_fond();
	dessin_texte_main("ULTRON",couleur_font_main,-2,600,175);
	dessin_menu();
	SDL_RenderPresent(renduPrincipale_main);
}


int input_main(){
	if(SDL_PollEvent(&touche_main)){
		switch(touche_main.type){
			case SDL_MOUSEMOTION:
				input_main();
				break;
			case SDL_QUIT:
				fermeture_sdl_main();
				exit(EXIT_SUCCESS);
				break;

			case SDL_KEYDOWN:
				if (touche_main.key.keysym.sym == SDLK_UP ){
					if (etat_main > 1){
						etat_main--;
					}

				}else if(touche_main.key.keysym.sym == SDLK_DOWN ){
					if (etat_main < 4){
						etat_main++;
					}

				}else if(touche_main.key.keysym.sym == SDLK_ESCAPE){
					fermeture_sdl_main();
					exit(EXIT_SUCCESS);

				}else if(touche_main.key.keysym.sym == SDLK_RETURN){
					if (etat_main == 1){
						survie();
					}else if(etat_main == 2){

					}else if(etat_main == 3){
						
					}else if(etat_main == 4){
						fermeture_sdl_main();
					}
				}
				break;
			}
		}
	}

int delay_game_main(){
	int lastTicks_survie = 0;
	int maxFPS_survie = 15;
	int delay_survie = 0;

	lastTicks_survie = SDL_GetTicks();
	delay_survie = 1000/maxFPS_survie-SDL_GetTicks()+lastTicks_survie;
	if(delay_survie>0){
		SDL_Delay(delay_survie);
	}
}

int fermeture_sdl_main(){
	SDL_DestroyRenderer(renduPrincipale_main);
	SDL_DestroyWindow(fenetrePrincipale_main);
	SDL_Quit();
	exit(EXIT_SUCCESS);
	return 0;
}

int get_screensize_main(){
	SDL_GetWindowSize(fenetrePrincipale_main, &width_windows_main, &height_windows_main);
	return 0;
}


int init_main(){

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

	if(SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_SHOWN, &fenetrePrincipale_main, &renduPrincipale_main) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_main);
    SDL_RenderPresent(renduPrincipale_main);

    SDL_SetWindowTitle(fenetrePrincipale_main, "Ultron");

    font_general_main = TTF_OpenFont("./Font/font.ttf", 16);
}