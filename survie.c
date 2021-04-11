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

int direction_survie = 0;
int size_main_survie = 32;

int width_windows_survie = 0;
int height_windows_survie = 0;

int score_survie = 0;

int terrain_x_survie = 0;
int terrain_y_survie = 0;

position posi_main_survie = {0,0};

SDL_Window* fenetrePrincipale_survie = NULL;
SDL_Renderer* renduPrincipale_survie = NULL;

SDL_Surface* main_surface_survie = NULL;
SDL_Texture* main_texture_survie = NULL;

SDL_Surface* background_score_surface_survie = NULL;
SDL_Texture* background_score_texture_survie = NULL;

TTF_Font* font_general_survie = NULL;

Mix_Music* music_de_fond_survie = NULL;
Mix_Chunk* explosion_survie = NULL;


int survie(){
	init_survie();
	get_screensize_survie();
	set_survie();
	play_musique_survie();
	while(1){
		input_survie();
		update_survie();
		set_survie();
		delay_game_survie();
		SDL_Log("1 - debug : direction_survie = %d , Position x = %d , position y = %d \n 2 - debug : width_windows_survie : %d height_windows_survie : %d \n 3 - debug : terrain_x_survie : %d terrain_y_survie : %d",direction_survie, posi_main_survie.x, posi_main_survie.y,width_windows_survie,height_windows_survie,terrain_x_survie,terrain_y_survie);
	}
	return 0;
}

int dessin_main_survie(){
	SDL_Rect dest = { posi_main_survie.x,posi_main_survie.y, main_surface_survie->w, main_surface_survie->h};
	SDL_RenderCopy(renduPrincipale_survie,main_texture_survie,NULL,&dest);
	return 0;
}

int dessin_background_score_survie(){
	SDL_Rect dest = { terrain_x_survie+1, 0, width_windows_survie - terrain_x_survie , height_windows_survie};
	SDL_RenderCopy(renduPrincipale_survie,background_score_texture_survie,NULL,&dest);
	return 0;
}

int dessin_score_survie(){
	char score_texte_survie[20];
	SDL_Color couleur_font_survie = {255, 255, 255};

	sprintf(score_texte_survie,"Score : %d", score_survie);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_survie,score_texte_survie, couleur_font_survie);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_survie, score_surface);

	SDL_Rect dest = {terrain_x_survie+10,10,175,50};

	SDL_RenderCopy(renduPrincipale_survie, score_texture, NULL, &dest);
}

int dessin_game_over_survie(){
	char game_over_texte_survie[43];
	SDL_Color couleur_font_survie = {255, 255, 255};

	sprintf(game_over_texte_survie,"GAME OVER !!! SCORE FINAL : %d ", score_survie);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_survie,game_over_texte_survie, couleur_font_survie);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_survie, score_surface);

	int size_game_over_x = 325*4;
	int size_game_over_y = 50*4;
	
	SDL_Rect dest = {(terrain_x_survie - size_game_over_x)/2,(terrain_y_survie - size_game_over_y*1.5)/2 ,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale_survie, score_texture, NULL, &dest);
}

int dessin_fond_survie(){
	int taille_score_survie = 6;
	SDL_SetRenderDrawColor(renduPrincipale_survie,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_survie);
	SDL_SetRenderDrawColor(renduPrincipale_survie,77, 77, 77, 255);

	terrain_x_survie = width_windows_survie - taille_score_survie*size_main_survie;
	terrain_y_survie = height_windows_survie;

	for(int x = 0; x <= terrain_x_survie; x += size_main_survie){
		SDL_RenderDrawLine(renduPrincipale_survie, x, 0, x, height_windows_survie);
	}

	for(int y = 0; y < terrain_y_survie; y += size_main_survie){
		SDL_RenderDrawLine(renduPrincipale_survie, 0, y, terrain_x_survie, y);
	}

	SDL_RenderPresent(renduPrincipale_survie);
	return 0;
}

int set_survie(){
	if(direction_survie && direction_survie !=6){
		dessin_main_survie();
		dessin_background_score_survie();
		dessin_score_survie();
		SDL_RenderPresent(renduPrincipale_survie);
	}else if(direction_survie == 6){
		dessin_fond_survie();
		dessin_background_score_survie();
		dessin_score_survie();
		dessin_game_over_survie();
		SDL_RenderPresent(renduPrincipale_survie);
	}else{
		dessin_fond_survie();
		dessin_background_score_survie();
		dessin_main_survie();
		dessin_score_survie();
		SDL_RenderPresent(renduPrincipale_survie);
	}
}

int init_survie(){

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

	if(SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_SHOWN, &fenetrePrincipale_survie, &renduPrincipale_survie) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_survie);
    SDL_RenderPresent(renduPrincipale_survie);

    SDL_SetWindowTitle(fenetrePrincipale_survie, "Ultron");

	main_surface_survie = SDL_LoadBMP("./Sprites/main.bmp");
	main_texture_survie = SDL_CreateTextureFromSurface(renduPrincipale_survie,main_surface_survie);

	background_score_surface_survie = SDL_LoadBMP("./Sprites/background_score.bmp");
	background_score_texture_survie = SDL_CreateTextureFromSurface(renduPrincipale_survie,background_score_surface_survie);

	font_general_survie = TTF_OpenFont("./Font/font.ttf", 16);
}

int input_survie(){

	SDL_Event touche_survie;

	if(SDL_PollEvent(&touche_survie)){
		switch(touche_survie.type){
			case SDL_MOUSEMOTION:
				input_survie();
				break;
			case SDL_QUIT:
				fermeture_sdl_survie();
				exit(EXIT_SUCCESS);
				break;

			case SDL_KEYDOWN:
				if (touche_survie.key.keysym.sym == SDLK_z && direction_survie !=6){
					direction_survie = 1;

				}else if(touche_survie.key.keysym.sym == SDLK_s && direction_survie !=6){
					direction_survie = 2;

				}else if(touche_survie.key.keysym.sym == SDLK_d && direction_survie !=6){
					direction_survie = 3;
					
				}else if(touche_survie.key.keysym.sym == SDLK_q && direction_survie !=6){
					direction_survie = 4;
					
				}else if(touche_survie.key.keysym.sym == SDLK_SPACE && direction_survie !=6){
					direction_survie = 5;

				}else if(touche_survie.key.keysym.sym == SDLK_ESCAPE){
					fermeture_sdl_survie();
					exit(EXIT_SUCCESS);
				}
				break;
			}
		}
	}


int update_survie(){
	switch(direction_survie){
		case 1:
			if(posi_main_survie.y<0){
				posi_main_survie.y = 0;
			}else if(posi_main_survie.y>0){
				posi_main_survie.y-=size_main_survie;
			}
			tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
			break;
		case 2:
			if(posi_main_survie.y>terrain_y_survie-size_main_survie){
				posi_main_survie.y = terrain_y_survie-size_main_survie;
			}else if(posi_main_survie.y<terrain_y_survie-size_main_survie){
				posi_main_survie.y+=size_main_survie;
			}
			tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
			break;
		case 3:
			if(posi_main_survie.x>terrain_x_survie-size_main_survie){
				posi_main_survie.x = terrain_x_survie-size_main_survie;
			}else if(posi_main_survie.x<terrain_x_survie-size_main_survie){
				posi_main_survie.x+=size_main_survie;
			}
			tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
			break;
		case 4:
			if(posi_main_survie.x<0){
				posi_main_survie.x = 0;
			}else if(posi_main_survie.x>0){
				posi_main_survie.x-=size_main_survie;
			}
			tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
			break;
		default:
			break;
	}
	if(direction_survie != 5){
		if(direction_survie != 0){
			if (direction_survie != 6){
				score_survie++;
			}
		}
	}

	return 0;
}

int tab_deplacement_survie(int x, int y){
	int tableau_deplacement_survie[width_windows_survie][height_windows_survie];

	if(tableau_deplacement_survie[x][y] == 1){
		direction_survie = 6;
		Mix_FreeMusic(music_de_fond_survie);
		play_explosion_survie();
	}else if(tableau_deplacement_survie[x][y] == 0){
		tableau_deplacement_survie[x][y] = 1;
	}
	return 0;
}

int delay_game_survie(){
	int lastTicks_survie = 0;
	int maxFPS_survie = 15;
	int delay_survie = 0;

	lastTicks_survie = SDL_GetTicks();
	delay_survie = 1000/maxFPS_survie-SDL_GetTicks()+lastTicks_survie;
	if(delay_survie>0){
		SDL_Delay(delay_survie);
	}
}

int fermeture_sdl_survie(){
	SDL_DestroyRenderer(renduPrincipale_survie);
	SDL_DestroyWindow(fenetrePrincipale_survie);
	Mix_FreeChunk(explosion_survie);
	SDL_Quit();
	return 0;
}

int get_screensize_survie(){
	SDL_GetWindowSize(fenetrePrincipale_survie, &width_windows_survie, &height_windows_survie);
	return 0;
}

int play_musique_survie(){
	music_de_fond_survie = Mix_LoadMUS("../../Documents/music.mp3");
	Mix_PlayMusic(music_de_fond_survie, -1);
}

int play_explosion_survie(){
	explosion_survie = Mix_LoadWAV("../../Documents/explosion.wav");
	Mix_PlayChannel(-1, explosion_survie, 0);
}