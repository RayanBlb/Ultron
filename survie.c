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

#define START 0
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define PAUSE 5
#define GAME_OVER 6

liste *pFirst_survie = NULL;

int etat_survie = START;

int width_windows_survie = 0;
int height_windows_survie = 0;

int score_survie = 0;

int terrain_x_survie = 0;
int terrain_y_survie = 0;

int size_main_survie = 32;//8/16/32

int **tableau_deplacement_survie;

char nom_high_score_survie[30];

position_survie posi_main_survie = {0,0};

SDL_Window *fenetrePrincipale_survie = NULL;
SDL_Renderer *renduPrincipale_survie = NULL;

SDL_Surface *main_surface_survie = NULL;
SDL_Texture *main_texture_survie = NULL;

SDL_Surface *background_score_surface_survie = NULL;
SDL_Texture *background_score_texture_survie = NULL;

SDL_Surface *background_high_score_surface_survie = NULL;
SDL_Texture *background_high_score_texture_survie = NULL;

TTF_Font *font_general_survie = NULL;

Mix_Music *music_de_fond_survie = NULL;
Mix_Chunk *explosion_survie = NULL;

//Boucle principale
int survie(int mode_difficulte){
	init_survie();
	SDL_GetWindowSize(fenetrePrincipale_survie, &width_windows_survie, &height_windows_survie);
	reinitialisation_survie(mode_difficulte);
	allocation_tableau_outils(&tableau_deplacement_survie,width_windows_survie,height_windows_survie);
	initialisation_position_main_survie();
	play_musique_outils(&music_de_fond_survie);
	set_start_survie();
	while(etat_survie != GAME_OVER){
		input_survie();
		update_survie();
		set_survie();
		delay_game_outils(etat_survie,0);
		//SDL_Log("1 - debug : etat_survie = %d , Position x = %d , position y = %d \n 2 - debug : width_windows_survie : %d height_windows_survie : %d \n 3 - debug : terrain_x_survie : %d terrain_y_survie : %d",etat_survie, posi_main_survie.x, posi_main_survie.y,width_windows_survie,height_windows_survie,terrain_x_survie,terrain_y_survie);
	}
	set_game_over_survie();
	return 0;
}
/*------------------------------------------*/

//Fonction d'affichage en fonction de l'état de survie
int set_survie(){
	if(etat_survie && etat_survie != GAME_OVER && etat_survie != PAUSE){
		dessin_main_survie();
		dessin_background_score_survie();
		dessin_score_survie();
		SDL_RenderPresent(renduPrincipale_survie);
	}else if (etat_survie == PAUSE){
		dessin_background_score_survie();
		dessin_score_survie();
		dessin_pause_survie();
		SDL_RenderPresent(renduPrincipale_survie);
	}
}

int set_start_survie(){
	if(etat_survie == START){
		dessin_fond_survie();
		dessin_background_score_survie();
		dessin_main_survie();
		dessin_score_survie();
		SDL_RenderPresent(renduPrincipale_survie);
	}
}

int set_game_over_survie(){
	if(etat_survie == GAME_OVER){
		dessin_fond_survie();
		dessin_background_score_survie();
	}
	while(1){
		dessin_background_high_score_survie();
		input_high_score_survie();
		dessin_high_score_survie();
		dessin_score_survie();
		dessin_game_over_survie();
		SDL_RenderPresent(renduPrincipale_survie);
		delay_game_outils(etat_survie,0);
	}
}
/*------------------------------------------*/

//Fonction de réinitialisation des variable global
int reinitialisation_survie(int mode_difficulte){
	etat_survie = START;
	score_survie = 0;

	if(mode_difficulte == 1){
		size_main_survie = 8;
	}else if(mode_difficulte == 2){
		size_main_survie = 16;
	}else if(mode_difficulte == 3){
		size_main_survie = 32;
	}

	if(tableau_deplacement_survie){
		free_tableau_outils(&tableau_deplacement_survie,width_windows_survie);
	}

	strcpy(nom_high_score_survie,"");

	return 0;
}

int initialisation_position_main_survie(){
	int x = (rand() % (45))*size_main_survie;
	int y = (rand() % (25))*size_main_survie;
	posi_main_survie.x = x;
	posi_main_survie.y = y;

	tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
}
/*------------------------------------------*/

//Fonction de dessin
int dessin_main_survie(){
	SDL_Rect dest = { posi_main_survie.x,posi_main_survie.y, size_main_survie, size_main_survie};
	SDL_RenderCopy(renduPrincipale_survie,main_texture_survie,NULL,&dest);
	return 0;
}

int dessin_background_score_survie(){
	SDL_Rect dest = { terrain_x_survie+1, 0, width_windows_survie - terrain_x_survie , height_windows_survie};
	SDL_RenderCopy(renduPrincipale_survie,background_score_texture_survie,NULL,&dest);
	return 0;
}

int dessin_score_survie(){
	char score_texte[20];
	SDL_Color couleur_font = {255, 255, 255};

	sprintf(score_texte,"Score : %d", score_survie);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_survie,score_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_survie, score_surface);

	SDL_Rect dest = {terrain_x_survie+10,10,175,50};

	SDL_RenderCopy(renduPrincipale_survie, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_game_over_survie(){
	char game_over_texte[43];
	SDL_Color couleur_font = {255, 255, 255};

	sprintf(game_over_texte,"GAME OVER !!! SCORE FINAL : %d ", score_survie);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_survie,game_over_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_survie, score_surface);

	int size_game_over_x = 325*4;
	int size_game_over_y = 50*4;

	int position_x = (terrain_x_survie - size_game_over_x)/2;
	int position_y = (terrain_y_survie - size_game_over_y*2)/2;
	
	SDL_Rect dest = {position_x,position_y,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale_survie, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_pause_survie(){
	SDL_Color couleur_font = {255, 255, 255};

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_survie,"PAUSE", couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_survie, score_surface);

	SDL_Rect dest = {terrain_x_survie+10,(terrain_y_survie-50)/2,175,50};

	SDL_RenderCopy(renduPrincipale_survie, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_fond_survie(){
	int taille_score = 210/size_main_survie;

	SDL_SetRenderDrawColor(renduPrincipale_survie,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_survie);
	SDL_SetRenderDrawColor(renduPrincipale_survie,77, 77, 77, 255);

	terrain_x_survie = width_windows_survie - taille_score*size_main_survie;
	terrain_y_survie = height_windows_survie;

	for(int x = 0; x <= terrain_x_survie; x += size_main_survie){
		SDL_RenderDrawLine(renduPrincipale_survie, x, 0, x, height_windows_survie);
	}

	for(int y = 0; y < terrain_y_survie; y += size_main_survie){
		SDL_RenderDrawLine(renduPrincipale_survie, 0, y, terrain_x_survie, y);
	}

	return 0;
}

int dessin_high_score_survie(){
	char game_over_texte[43];
	SDL_Color couleur_font = {255, 255, 255};

	sprintf(game_over_texte,"%s", nom_high_score_survie);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_survie,game_over_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_survie, score_surface);

	int size_game_over_x = 100*compte_nom_high_score_outils(nom_high_score_survie);
	int size_game_over_y = 175;

	int position_x = (terrain_x_survie - size_game_over_x)/2;
	int position_y = (terrain_y_survie + size_game_over_y*0.5)/2;	
	
	SDL_Rect dest = {position_x,position_y,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale_survie, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_background_high_score_survie(){
	int size_game_over_x = 1100;
	int size_game_over_y = 175;

	int position_x = (terrain_x_survie - size_game_over_x)/2;
	int position_y = (terrain_y_survie + size_game_over_y*0.5)/2;

	SDL_Rect dest = {position_x, position_y, size_game_over_x, size_game_over_y};
	SDL_RenderCopy(renduPrincipale_survie,background_high_score_texture_survie,NULL,&dest);
	return 0;
}
/*------------------------------------------*/

//Fonction relative au input du joueur
int input_survie(){
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_survie();
				break;
			case SDL_QUIT:
				fermeture_sdl_outils(fenetrePrincipale_survie,renduPrincipale_survie,explosion_survie,main_surface_survie,main_texture_survie,NULL,NULL);
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_z && etat_survie != GAME_OVER){
					etat_survie = UP;

				}else if(touche.key.keysym.sym == SDLK_s && etat_survie != GAME_OVER){
					etat_survie = DOWN;

				}else if(touche.key.keysym.sym == SDLK_d && etat_survie != GAME_OVER){
					etat_survie = RIGHT;
					
				}else if(touche.key.keysym.sym == SDLK_q && etat_survie != GAME_OVER){
					etat_survie = LEFT;
					
				}else if(touche.key.keysym.sym == SDLK_SPACE && etat_survie != GAME_OVER){
					etat_survie = PAUSE;

				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_outils(fenetrePrincipale_survie,renduPrincipale_survie,music_de_fond_survie,main_surface_survie,main_texture_survie, etat_survie,NULL,NULL);
				}
				break;
			}
		}
	}

int input_high_score_survie(){
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_menu();
				break;
			case SDL_QUIT:
				fermeture_sdl_outils(fenetrePrincipale_survie,renduPrincipale_survie,explosion_survie,main_surface_survie,main_texture_survie,NULL,NULL);
				break;

			case SDL_KEYDOWN:
				if(touche.key.keysym.sym != SDLK_RGUI && touche.key.keysym.sym != SDLK_LGUI && touche.key.keysym.sym != SDLK_CAPSLOCK && touche.key.keysym.sym != SDLK_HOME && touche.key.keysym.sym != SDLK_PAGEDOWN && touche.key.keysym.sym != SDLK_PAGEUP && touche.key.keysym.sym != SDLK_END && touche.key.keysym.sym != SDLK_BACKSPACE && touche.key.keysym.sym != SDLK_DELETE && touche.key.keysym.sym != SDLK_INSERT && touche.key.keysym.sym != SDLK_TAB && touche.key.keysym.sym != SDLK_RIGHT && touche.key.keysym.sym != SDLK_RIGHT && touche.key.keysym.sym != SDLK_LEFT && touche.key.keysym.sym != SDLK_DOWN && touche.key.keysym.sym != SDLK_UP && touche.key.keysym.sym != SDLK_LCTRL && touche.key.keysym.sym != SDLK_RCTRL && touche.key.keysym.sym != SDLK_LALT && touche.key.keysym.sym != SDLK_RALT && touche.key.keysym.sym != SDLK_RSHIFT && touche.key.keysym.sym != SDLK_LSHIFT && touche.key.keysym.sym != SDLK_ESCAPE && touche.key.keysym.sym != SDLK_RETURN && touche.key.keysym.sym != SDLK_BACKSPACE && touche.key.keysym.sym != SDLK_LSHIFT && touche.key.keysym.sym != SDLK_RSHIFT && touche.key.keysym.sym != SDLK_SPACE  && compte_nom_high_score_outils(nom_high_score_survie) < 10){
					strcat(nom_high_score_survie,SDL_GetKeyName(touche.key.keysym.sym));
				}else if(touche.key.keysym.sym == SDLK_BACKSPACE && compte_nom_high_score_outils(nom_high_score_survie) > 0){
					int compteur = compte_nom_high_score_outils(nom_high_score_survie);
					compteur--;
					strcpy(&nom_high_score_survie[compteur],"");
				}else if(touche.key.keysym.sym == SDLK_RETURN){
					read_file_high_score_outils("./score_survie.txt",nom_high_score_survie,score_survie,pFirst_survie);
					switch_screen_outils(fenetrePrincipale_survie,renduPrincipale_survie,music_de_fond_survie,main_surface_survie,main_texture_survie, etat_survie,NULL,NULL);
				}else if(touche.key.keysym.sym == SDLK_LSHIFT){
				}else if(touche.key.keysym.sym == SDLK_RSHIFT){
				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_outils(fenetrePrincipale_survie,renduPrincipale_survie,music_de_fond_survie,main_surface_survie,main_texture_survie, etat_survie,NULL,NULL);
				}
				break;
			}
		}
	}
/*------------------------------------------*/

//Fonction gestion enregistrement de déplacement
int update_survie(){

	switch(etat_survie){
		case UP:
			if(posi_main_survie.y<0){
				posi_main_survie.y = 0;
			}else if(posi_main_survie.y>0){
				posi_main_survie.y-=size_main_survie;
			}
			tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
			break;
		case DOWN:
			if(posi_main_survie.y>terrain_y_survie-size_main_survie){
				posi_main_survie.y = terrain_y_survie-size_main_survie;
			}else if(posi_main_survie.y<terrain_y_survie-size_main_survie){
				posi_main_survie.y+=size_main_survie;
			}
			tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
			break;
		case RIGHT:
			if(posi_main_survie.x>terrain_x_survie-size_main_survie){
				posi_main_survie.x = terrain_x_survie-size_main_survie;
			}else if(posi_main_survie.x<terrain_x_survie-size_main_survie){
				posi_main_survie.x+=size_main_survie;
			}
			tab_deplacement_survie(posi_main_survie.x,posi_main_survie.y);
			break;
		case LEFT:
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

	if(etat_survie != PAUSE && etat_survie != START && etat_survie != GAME_OVER){
		score_survie++;
	}

	return 0;
}

int tab_deplacement_survie(int x, int y){
	if(tableau_deplacement_survie[x][y] == 1){
		etat_survie = GAME_OVER;
		Mix_FreeMusic(music_de_fond_survie);
		play_explosion_outils(&explosion_survie);
	}else if(tableau_deplacement_survie[x][y] == 0){
		tableau_deplacement_survie[x][y] = 1;
	}
	return 0;
}

/*------------------------------------------*/

//Fonction initialisation
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

	if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_SHOWN, &fenetrePrincipale_survie, &renduPrincipale_survie) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_survie);
    SDL_RenderPresent(renduPrincipale_survie);

    SDL_SetWindowTitle(fenetrePrincipale_survie, "Ultron");

    SDL_Surface* icon_programme_survie = SDL_LoadBMP("./Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_survie, icon_programme_survie);

	main_surface_survie = SDL_LoadBMP("./Sprites/main.bmp");
	main_texture_survie = SDL_CreateTextureFromSurface(renduPrincipale_survie,main_surface_survie);

	background_score_surface_survie = SDL_LoadBMP("./Sprites/background_score.bmp");
	background_score_texture_survie = SDL_CreateTextureFromSurface(renduPrincipale_survie,background_score_surface_survie);

	background_high_score_surface_survie = SDL_LoadBMP("./Sprites/background_high_score.bmp");
	background_high_score_texture_survie = SDL_CreateTextureFromSurface(renduPrincipale_survie,background_high_score_surface_survie);

	font_general_survie = TTF_OpenFont("./Font/font.ttf", 16);
}
/*------------------------------------------*/