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

#define START 1
#define PAUSE 2
#define IN_GAME 3

#define UP_1 4
#define DOWN_1 5
#define RIGHT_1 6
#define LEFT_1 7

#define UP_2 8
#define DOWN_2 9
#define RIGHT_2 10
#define LEFT_2 11

#define GAME_OVER_1_win 12
#define GAME_OVER_1_suicide 13
#define GAME_OVER_2_win 14
#define GAME_OVER_2_suicide 15

liste *pFirst_ia = NULL;

int etat_ia = START;
int etat_main_ia = START;
int etat_deuxieme_ia = START;

int compteur_miroire_ia = 0;

int timer_deplacement_ia = 0;
int changement_direction_ia = 0;
int start_ia = 0;

int prev_etat_main_ia = START;
int prev_etat_deuxieme_ia = START;

int width_windows_ia = 0;
int height_windows_ia = 0;

int score_ia = 0;

int terrain_x_ia = 0;
int terrain_y_ia = 0;

int size_main_ia = 32;//8/16/32

int **tableau_deplacement_ia;

char nom_high_score_ia[30];

position_ia posi_main_ia = {0,0};
position_ia posi_deuxieme_ia = {0,0};

SDL_Window *fenetrePrincipale_ia = NULL;
SDL_Renderer *renduPrincipale_ia = NULL;

SDL_Surface *main_surface_ia = NULL;
SDL_Texture *main_texture_ia = NULL;

SDL_Surface *deuxieme_surface_ia = NULL;
SDL_Texture *deuxieme_texture_ia = NULL;

SDL_Surface *background_score_surface_ia = NULL;
SDL_Texture *background_score_texture_ia = NULL;

SDL_Surface *background_high_score_surface_ia = NULL;
SDL_Texture *background_high_score_texture_ia = NULL;

TTF_Font *font_general_ia = NULL;

Mix_Music *music_de_fond_ia = NULL;
Mix_Chunk *explosion_ia = NULL;

//Boucle principale
int ia(int mode_difficulte,int mode_ia){
	init_ia();
	SDL_GetWindowSize(fenetrePrincipale_ia, &width_windows_ia, &height_windows_ia);
	reinitialisation_ia(mode_difficulte);
	allocation_tableau_outils(&tableau_deplacement_ia,width_windows_ia,height_windows_ia);
	initialisation_position_main_ia(mode_ia);
	play_musique_outils(&music_de_fond_ia);
	set_start_ia();
	while(etat_ia < GAME_OVER_1_win){
		choix_mode_ia(mode_ia);
		input_ia();
		update_ia();
		set_ia();
		delay_game_outils(etat_ia,1);
		//SDL_Log("1 - debug : etat_ia = %d , etat_main_ia = %d , etat_deuxieme_ia = %d\n 2 - debug : terrain_x_ia : %d , terrain_y_ia : %d \n",etat_ia,etat_main_ia,etat_deuxieme_ia,terrain_x_ia,terrain_y_ia);
	}
	set_game_over_ia();
	return 0;
}
/*------------------------------------------*/

//Fonction d'affichage en fonction de l'état de ia
int set_ia(){
	if(etat_ia && etat_ia < GAME_OVER_1_win && etat_ia != PAUSE){
		dessin_main_ia();
		dessin_deuxieme_ia();
		dessin_background_score_ia();
		dessin_score_ia();
		SDL_RenderPresent(renduPrincipale_ia);
	}else if (etat_main_ia == PAUSE){
		dessin_background_score_ia();
		dessin_score_ia();
		dessin_pause_ia();
		SDL_RenderPresent(renduPrincipale_ia);
	}
}

int set_start_ia(){
	if(etat_ia == START){
		dessin_fond_ia();
		dessin_background_score_ia();
		dessin_main_ia();
		dessin_deuxieme_ia();
		dessin_score_ia();
		SDL_RenderPresent(renduPrincipale_ia);
	}
}

int set_game_over_ia(){
	if(etat_ia >= GAME_OVER_1_win){
		dessin_fond_ia();
		dessin_background_score_ia();
	}
	while(1){
		dessin_background_high_score_ia();
		input_high_score_ia();
		dessin_high_score_ia();
		dessin_score_ia();
		dessin_game_over_ia();
		SDL_RenderPresent(renduPrincipale_ia);
		delay_game_outils(etat_ia,1);
	}
}
/*------------------------------------------*/

//Fonction de réinitialisation des variable global
int reinitialisation_ia(int mode_difficulte){
	if(mode_difficulte == 1){
		size_main_ia = 8;
	}else if(mode_difficulte == 2){
		size_main_ia = 16;
	}else if(mode_difficulte == 3){
		size_main_ia = 32;
	}

	start_ia = 0;

	etat_ia = START;
	etat_main_ia = START;
	etat_deuxieme_ia = START;

	score_ia = 0;

	if(tableau_deplacement_ia){
		free_tableau_outils(&tableau_deplacement_ia,width_windows_ia);
	}

	strcpy(nom_high_score_ia,"");

	int taille_score = 210/size_main_ia;

	terrain_x_ia = width_windows_ia - taille_score*size_main_ia;
	terrain_y_ia = height_windows_ia;

	return 0;
}

int initialisation_position_main_ia(int mode_ia){
	srand(time(0));
	
	if(mode_ia == 3){
		posi_main_ia.x = 0;
		posi_main_ia.y = 0;

		posi_deuxieme_ia.x = terrain_x_ia-size_main_ia;
		posi_deuxieme_ia.y = 0;
	}else{
		int x = (rand() % (45 + 1 - 23) + 23)*size_main_ia;
		int y = (rand() % (25 + 1 - 13) + 13)*size_main_ia;
		int i = (rand() % (22 + 1 - 10) + 10)*size_main_ia;
		int j = (rand() % (12 + 1 - 5) + 5)*size_main_ia;

		posi_main_ia.x = i;
		posi_main_ia.y = j;

		posi_deuxieme_ia.x = x;
		posi_deuxieme_ia.y = y;
	}

	tab_deplacement_ia(posi_main_ia.x,posi_main_ia.y,1);
	tab_deplacement_ia(posi_deuxieme_ia.x,posi_deuxieme_ia.y,2);
}
/*------------------------------------------*/

//Fonction de dessin
int dessin_main_ia(){
	SDL_Rect dest = { posi_main_ia.x,posi_main_ia.y, size_main_ia, size_main_ia};
	SDL_RenderCopy(renduPrincipale_ia,main_texture_ia,NULL,&dest);
	return 0;
}

int dessin_deuxieme_ia(){
	SDL_Rect dest = { posi_deuxieme_ia.x,posi_deuxieme_ia.y, size_main_ia, size_main_ia};
	SDL_RenderCopy(renduPrincipale_ia,deuxieme_texture_ia,NULL,&dest);
	return 0;
}

int dessin_background_score_ia(){
	SDL_Rect dest = { terrain_x_ia+1, 0, width_windows_ia - terrain_x_ia , height_windows_ia};
	SDL_RenderCopy(renduPrincipale_ia,background_score_texture_ia,NULL,&dest);
	return 0;
}

int dessin_score_ia(){
	char score_texte[20];
	SDL_Color couleur_font = {255, 255, 255};

	sprintf(score_texte,"Score : %d", score_ia);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_ia,score_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_ia, score_surface);

	SDL_Rect dest = {terrain_x_ia+10,10,175,50};

	SDL_RenderCopy(renduPrincipale_ia, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_game_over_ia(){
	char game_over_texte[43];
	SDL_Color couleur_font = {255, 255, 255};

	if(etat_ia == GAME_OVER_1_win){
		sprintf(game_over_texte,"WHITE WIN !!! SCORE FINAL : %d ", score_ia);
	}else if(etat_ia == GAME_OVER_2_win){
		sprintf(game_over_texte,"RED WIN !!! SCORE FINAL : %d ", score_ia);
	}else if(etat_ia == GAME_OVER_1_suicide){
		sprintf(game_over_texte,"RED WIN !!! SCORE FINAL : %d ", score_ia);
	}else if(etat_ia == GAME_OVER_2_suicide){
		sprintf(game_over_texte,"WHITE WIN !!! SCORE FINAL : %d ", score_ia);
	}

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_ia,game_over_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_ia, score_surface);

	int size_game_over_x = 325*4;
	int size_game_over_y = 50*4;

	int position_x = (terrain_x_ia - size_game_over_x)/2;
	int position_y = (terrain_y_ia - size_game_over_y*2)/2;
	
	SDL_Rect dest = {position_x,position_y,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale_ia, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_pause_ia(){
	SDL_Color couleur_font = {255, 255, 255};

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_ia,"PAUSE", couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_ia, score_surface);

	SDL_Rect dest = {terrain_x_ia+10,(terrain_y_ia-50)/2,175,50};

	SDL_RenderCopy(renduPrincipale_ia, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_fond_ia(){

	SDL_SetRenderDrawColor(renduPrincipale_ia,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_ia);
	SDL_SetRenderDrawColor(renduPrincipale_ia,77, 77, 77, 255);

	for(int x = 0; x <= terrain_x_ia; x += size_main_ia){
		SDL_RenderDrawLine(renduPrincipale_ia, x, 0, x, height_windows_ia);
	}

	for(int y = 0; y < terrain_y_ia; y += size_main_ia){
		SDL_RenderDrawLine(renduPrincipale_ia, 0, y, terrain_x_ia, y);
	}

	return 0;
}

int dessin_high_score_ia(){
	char game_over_texte[43];
	SDL_Color couleur_font = {255, 255, 255};

	sprintf(game_over_texte,"%s", nom_high_score_ia);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_ia,game_over_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_ia, score_surface);

	int size_game_over_x = 100*compte_nom_high_score_outils(nom_high_score_ia);
	int size_game_over_y = 175;

	int position_x = (terrain_x_ia - size_game_over_x)/2;
	int position_y = (terrain_y_ia + size_game_over_y*0.5)/2;	
	
	SDL_Rect dest = {position_x,position_y,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale_ia, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

int dessin_background_high_score_ia(){
	int size_game_over_x = 1100;
	int size_game_over_y = 175;

	int position_x = (terrain_x_ia - size_game_over_x)/2;
	int position_y = (terrain_y_ia + size_game_over_y*0.5)/2;

	SDL_Rect dest = {position_x, position_y, size_game_over_x, size_game_over_y};
	SDL_RenderCopy(renduPrincipale_ia,background_high_score_texture_ia,NULL,&dest);
	return 0;
}
/*------------------------------------------*/

//Fonction relative au input du joueur
int input_ia(){
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_ia();
				break;
			case SDL_QUIT:
				fermeture_sdl_outils(fenetrePrincipale_ia,renduPrincipale_ia,explosion_ia,main_surface_ia,main_texture_ia,deuxieme_surface_ia,deuxieme_texture_ia);
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_z && etat_ia < GAME_OVER_1_win){
					etat_main_ia = UP_1;

				}else if(touche.key.keysym.sym == SDLK_s && etat_ia < GAME_OVER_1_win){
					etat_main_ia = DOWN_1;

				}else if(touche.key.keysym.sym == SDLK_d && etat_ia < GAME_OVER_1_win){
					etat_main_ia = RIGHT_1;
					
				}else if(touche.key.keysym.sym == SDLK_q && etat_ia < GAME_OVER_1_win){
					etat_main_ia = LEFT_1;
					
				}else if(touche.key.keysym.sym == SDLK_SPACE && etat_ia < GAME_OVER_1_win){
					prev_etat_main_ia = etat_main_ia;
					prev_etat_deuxieme_ia = etat_deuxieme_ia;
					etat_main_ia = PAUSE;
					etat_deuxieme_ia = PAUSE;

				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_outils(fenetrePrincipale_ia,renduPrincipale_ia,music_de_fond_ia,main_surface_ia,main_texture_ia, etat_ia,deuxieme_surface_ia,deuxieme_texture_ia);
				}

				if(etat_ia == START && etat_main_ia != START){
					etat_ia = IN_GAME;
				}

				if(etat_main_ia == PAUSE && etat_deuxieme_ia != PAUSE){
					etat_main_ia = prev_etat_main_ia;
				}else if(etat_main_ia != PAUSE && etat_deuxieme_ia == PAUSE){
					etat_deuxieme_ia = prev_etat_deuxieme_ia;
				}
				
				break;
			}
		}
	}

int input_high_score_ia(){
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_menu();
				break;
			case SDL_QUIT:
				fermeture_sdl_outils(fenetrePrincipale_ia,renduPrincipale_ia,explosion_ia,main_surface_ia,main_texture_ia,deuxieme_surface_ia,deuxieme_texture_ia);
				break;

			case SDL_KEYDOWN:
				if(touche.key.keysym.sym != SDLK_RGUI && touche.key.keysym.sym != SDLK_LGUI && touche.key.keysym.sym != SDLK_CAPSLOCK && touche.key.keysym.sym != SDLK_HOME && touche.key.keysym.sym != SDLK_PAGEDOWN && touche.key.keysym.sym != SDLK_PAGEUP && touche.key.keysym.sym != SDLK_END && touche.key.keysym.sym != SDLK_BACKSPACE && touche.key.keysym.sym != SDLK_DELETE && touche.key.keysym.sym != SDLK_INSERT && touche.key.keysym.sym != SDLK_TAB && touche.key.keysym.sym != SDLK_RIGHT && touche.key.keysym.sym != SDLK_RIGHT && touche.key.keysym.sym != SDLK_LEFT && touche.key.keysym.sym != SDLK_DOWN && touche.key.keysym.sym != SDLK_UP && touche.key.keysym.sym != SDLK_LCTRL && touche.key.keysym.sym != SDLK_RCTRL && touche.key.keysym.sym != SDLK_LALT && touche.key.keysym.sym != SDLK_RALT && touche.key.keysym.sym != SDLK_RSHIFT && touche.key.keysym.sym != SDLK_LSHIFT && touche.key.keysym.sym != SDLK_ESCAPE && touche.key.keysym.sym != SDLK_RETURN && touche.key.keysym.sym != SDLK_BACKSPACE && touche.key.keysym.sym != SDLK_LSHIFT && touche.key.keysym.sym != SDLK_RSHIFT && touche.key.keysym.sym != SDLK_SPACE  && compte_nom_high_score_outils(nom_high_score_ia) < 10){
					strcat(nom_high_score_ia,SDL_GetKeyName(touche.key.keysym.sym));
				}else if(touche.key.keysym.sym == SDLK_BACKSPACE && compte_nom_high_score_outils(nom_high_score_ia) > 0){
					int compteur = compte_nom_high_score_outils(nom_high_score_ia);
					compteur--;
					strcpy(&nom_high_score_ia[compteur],"");
				}else if(touche.key.keysym.sym == SDLK_RETURN){
					read_file_high_score_outils("./score_ia.txt",nom_high_score_ia,score_ia,pFirst_ia);
					switch_screen_outils(fenetrePrincipale_ia,renduPrincipale_ia,music_de_fond_ia,main_surface_ia,main_texture_ia,etat_ia,deuxieme_surface_ia,deuxieme_texture_ia);
				}else if(touche.key.keysym.sym == SDLK_LSHIFT){
				}else if(touche.key.keysym.sym == SDLK_RSHIFT){
				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_outils(fenetrePrincipale_ia,renduPrincipale_ia,music_de_fond_ia,main_surface_ia,main_texture_ia, etat_ia,deuxieme_surface_ia,deuxieme_texture_ia);
				}
				break;
			}
		}
	}
/*------------------------------------------*/

//Fonction gestion enregistrement de déplacement
int update_ia(){

	switch(etat_main_ia){
		case UP_1:
			if(posi_main_ia.y<0){
				posi_main_ia.y = 0;
			}else if(posi_main_ia.y>0){
				posi_main_ia.y-=size_main_ia;
			}
			tab_deplacement_ia(posi_main_ia.x,posi_main_ia.y,1);
			break;
		case DOWN_1:
			if(posi_main_ia.y>terrain_y_ia-size_main_ia){
				posi_main_ia.y = terrain_y_ia-size_main_ia;
			}else if(posi_main_ia.y<terrain_y_ia-size_main_ia){
				posi_main_ia.y+=size_main_ia;
			}
			tab_deplacement_ia(posi_main_ia.x,posi_main_ia.y,1);
			break;
		case RIGHT_1:
			if(posi_main_ia.x>terrain_x_ia-size_main_ia){
				posi_main_ia.x = terrain_x_ia-size_main_ia;
			}else if(posi_main_ia.x<terrain_x_ia-size_main_ia){
				posi_main_ia.x+=size_main_ia;
			}
			tab_deplacement_ia(posi_main_ia.x,posi_main_ia.y,1);
			break;
		case LEFT_1:
			if(posi_main_ia.x<0){
				posi_main_ia.x = 0;
			}else if(posi_main_ia.x>0){
				posi_main_ia.x-=size_main_ia;
			}
			tab_deplacement_ia(posi_main_ia.x,posi_main_ia.y,1);
			break;
		default:
			break;
	}

	if(etat_ia < GAME_OVER_1_win){
		switch(etat_deuxieme_ia){
			case UP_2:
				if(posi_deuxieme_ia.y<0){
					posi_deuxieme_ia.y = 0;
				}else if(posi_deuxieme_ia.y>0){
					posi_deuxieme_ia.y-=size_main_ia;
				}
				tab_deplacement_ia(posi_deuxieme_ia.x,posi_deuxieme_ia.y,2);
				break;
			case DOWN_2:
				if(posi_deuxieme_ia.y>terrain_y_ia-size_main_ia){
					posi_deuxieme_ia.y = terrain_y_ia-size_main_ia;
				}else if(posi_deuxieme_ia.y<terrain_y_ia-size_main_ia){
					posi_deuxieme_ia.y+=size_main_ia;
				}
				tab_deplacement_ia(posi_deuxieme_ia.x,posi_deuxieme_ia.y,2);
				break;
			case RIGHT_2:
				if(posi_deuxieme_ia.x>terrain_x_ia-size_main_ia){
					posi_deuxieme_ia.x = terrain_x_ia-size_main_ia;
				}else if(posi_deuxieme_ia.x<terrain_x_ia-size_main_ia){
					posi_deuxieme_ia.x+=size_main_ia;
				}
				tab_deplacement_ia(posi_deuxieme_ia.x,posi_deuxieme_ia.y,2);
				break;
			case LEFT_2:
				if(posi_deuxieme_ia.x<0){
					posi_deuxieme_ia.x = 0;
				}else if(posi_deuxieme_ia.x>0){
					posi_deuxieme_ia.x-=size_main_ia;
				}
				tab_deplacement_ia(posi_deuxieme_ia.x,posi_deuxieme_ia.y,2);
				break;
			default:
				break;
			}
		}

	if((etat_deuxieme_ia != PAUSE || etat_main_ia != PAUSE) && (etat_deuxieme_ia != START || etat_main_ia != START) && etat_ia < GAME_OVER_1_win){
		score_ia++;
	}

	return 0;
}

int tab_deplacement_ia(int x, int y,int joueur){
	if(tableau_deplacement_ia[x][y] == 1 && joueur != 1){
		etat_ia = GAME_OVER_1_win;
		Mix_FreeMusic(music_de_fond_ia);
		play_explosion_outils(&explosion_ia);
		return 0;
	}else if(tableau_deplacement_ia[x][y] == 1 && joueur == 1){
		etat_ia = GAME_OVER_1_suicide;
		Mix_FreeMusic(music_de_fond_ia);
		play_explosion_outils(&explosion_ia);
		return 0;
	}else if(tableau_deplacement_ia[x][y] == 2 && joueur != 2){
		etat_ia = GAME_OVER_2_win;
		Mix_FreeMusic(music_de_fond_ia);
		play_explosion_outils(&explosion_ia);
		return 0;
	}else if(tableau_deplacement_ia[x][y] == 2 && joueur == 2){
		etat_ia = GAME_OVER_2_suicide;
		Mix_FreeMusic(music_de_fond_ia);
		play_explosion_outils(&explosion_ia);
		return 0;
	}else if(tableau_deplacement_ia[x][y] == 0 && joueur == 1){
		tableau_deplacement_ia[x][y] = 1;
		return 0;
	}else if(tableau_deplacement_ia[x][y] == 0 && joueur == 2){
		tableau_deplacement_ia[x][y] = 2;
		return 0;
	}
}

/*------------------------------------------*/

//Fonction initialisation
int init_ia(){

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

	if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_SHOWN, &fenetrePrincipale_ia, &renduPrincipale_ia) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_ia);
    SDL_RenderPresent(renduPrincipale_ia);

    SDL_SetWindowTitle(fenetrePrincipale_ia, "Ultron");

    SDL_Surface* icon_programme_ia = SDL_LoadBMP("./Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_ia, icon_programme_ia);

	main_surface_ia = SDL_LoadBMP("./Sprites/main.bmp");
	main_texture_ia = SDL_CreateTextureFromSurface(renduPrincipale_ia,main_surface_ia);

	deuxieme_surface_ia = SDL_LoadBMP("./Sprites/deuxieme.bmp");
	deuxieme_texture_ia = SDL_CreateTextureFromSurface(renduPrincipale_ia,deuxieme_surface_ia);

	background_score_surface_ia = SDL_LoadBMP("./Sprites/background_score.bmp");
	background_score_texture_ia = SDL_CreateTextureFromSurface(renduPrincipale_ia,background_score_surface_ia);

	background_high_score_surface_ia = SDL_LoadBMP("./Sprites/background_high_score.bmp");
	background_high_score_texture_ia = SDL_CreateTextureFromSurface(renduPrincipale_ia,background_high_score_surface_ia);

	font_general_ia = TTF_OpenFont("./Font/font.ttf", 16);
}
/*------------------------------------------*/

int chance_ia(int choix1,int choix2){
	if((rand() % (2 - 1 + 1)) + 1 == 1){
		return choix1;
	}else{
		return choix2;
	}
}

int choix_depart_ia(){
	srand(time(0));

	int coordonne_x = terrain_x_ia-posi_deuxieme_ia.x;
	int coordonne_y = terrain_y_ia-posi_deuxieme_ia.y;

	if((rand() % (2 - 1 + 1)) + 1 == 1){
		if(coordonne_x > posi_deuxieme_ia.x){
		etat_deuxieme_ia = RIGHT_2;
	}else if(coordonne_x < posi_deuxieme_ia.x){
		etat_deuxieme_ia = LEFT_2;
	}
	}else{
		if(coordonne_y > posi_deuxieme_ia.y){
		etat_deuxieme_ia = UP_2;
	}else if(coordonne_y < posi_deuxieme_ia.y){
		etat_deuxieme_ia = DOWN_2;
	}
	}
}
int brain_miroir_ia(){

	int decalage = 3;

	if(etat_main_ia == UP_1 && compteur_miroire_ia == 0){
		etat_deuxieme_ia = UP_2;
		compteur_miroire_ia = decalage;
	}else if(etat_main_ia == DOWN_1 && compteur_miroire_ia == 0){
		etat_deuxieme_ia = DOWN_2;
		compteur_miroire_ia = decalage;
	}else if(etat_main_ia == LEFT_1 && compteur_miroire_ia == 0){
		etat_deuxieme_ia = RIGHT_2;
		compteur_miroire_ia = decalage;
	}else if(etat_main_ia == RIGHT_1 && compteur_miroire_ia == 0){
		etat_deuxieme_ia = LEFT_2;
		compteur_miroire_ia = decalage;
	}else if(compteur_miroire_ia != 0){
		compteur_miroire_ia--;
	}
}

int brain_classique_ia(){
	int UP = 0;
	int DOWN = 0;
	int LEFT = 0;
	int RIGHT = 0;

	int vision = size_main_ia*5;

	if(posi_deuxieme_ia.y < vision){
		UP = 1;
	}else{
		if(etat_deuxieme_ia == UP_2){
		for (int i = 1; i < 2; ++i){
			if(tableau_deplacement_ia[posi_deuxieme_ia.x][posi_deuxieme_ia.y-size_main_ia*i] != 0){
				UP = 1;
			}
		}
	}
}

	if(posi_deuxieme_ia.y > terrain_y_ia-vision){
		DOWN = 1;
	}else{
		if(etat_deuxieme_ia == DOWN_2){
			for (int i = 1; i < 2; ++i){
				if(tableau_deplacement_ia[posi_deuxieme_ia.x][posi_deuxieme_ia.y+size_main_ia*i] != 0){
					DOWN = 1;
				}
			}
		}
	}

	if(posi_deuxieme_ia.x > terrain_x_ia-vision){
		RIGHT = 1;
	}else{
		if(etat_deuxieme_ia == RIGHT_2){
			for (int i = 1; i < 2; ++i){
				if(tableau_deplacement_ia[posi_deuxieme_ia.x+size_main_ia*i][posi_deuxieme_ia.y]  != 0){
					RIGHT = 1;
				}
			}
		}
	}

	if(posi_deuxieme_ia.x < vision){
		LEFT = 1;
	}else{
		if(etat_deuxieme_ia == LEFT_2){
		for (int i = 1; i < 2; ++i){
			if(tableau_deplacement_ia[posi_deuxieme_ia.x-size_main_ia*i][posi_deuxieme_ia.y]  != 0){
				LEFT = 1;
			}
		}
	}
}
	if(etat_ia == IN_GAME && etat_main_ia != PAUSE){
		if(etat_deuxieme_ia == UP_2){
			if(UP == 1){
				etat_deuxieme_ia = LEFT_2;
			}else if(UP == 1 && LEFT == 1){
				etat_deuxieme_ia = DOWN_2;
			}else if(UP == 1 && RIGHT == 1){
				etat_deuxieme_ia = DOWN_2;
			}
		}else if(etat_deuxieme_ia == DOWN_2){
			if(DOWN == 1){
				etat_deuxieme_ia = RIGHT_2;
			}else if(DOWN == 1 && LEFT == 1){
				etat_deuxieme_ia = RIGHT_2;
			}else if(DOWN == 1 && RIGHT == 1){
				etat_deuxieme_ia = LEFT_2;
			}
		}else if(etat_deuxieme_ia == LEFT_2){
			if(LEFT == 1){
				etat_deuxieme_ia = DOWN_2;
			}else if(LEFT == 1 && UP == 1){
				etat_deuxieme_ia = DOWN_2;
			}else if(LEFT == 1 && DOWN == 1){
				etat_deuxieme_ia = RIGHT_2;
			}
		}else if(etat_deuxieme_ia == RIGHT_2){
			if(RIGHT == 1){
				etat_deuxieme_ia = UP_2;
			}else if(RIGHT == 1 && UP == 1){
				etat_deuxieme_ia = UP_2;
			}else if(RIGHT == 1 && DOWN == 1){
				etat_deuxieme_ia = UP_2;
			}
		}else if(UP == 0 && DOWN == 0 && LEFT == 0 && RIGHT == 0 && start_ia != 0 && timer_deplacement_ia == 0){
			etat_deuxieme_ia = etat_deuxieme_ia;
		}else if(start_ia == 0){
			choix_depart_ia();
			start_ia = 1;
		}
	}
}

int brain_ia(){

	srand(time(0));

	int UP = 0;
	int DOWN = 0;
	int LEFT = 0;
	int RIGHT = 0;

	int vision = size_main_ia*2;

	if(posi_deuxieme_ia.y < vision){
		UP = 1;
	}else{
		if(etat_deuxieme_ia == UP_2){
		for (int i = 1; i < 2; ++i){
			if(tableau_deplacement_ia[posi_deuxieme_ia.x][posi_deuxieme_ia.y-size_main_ia*i] != 0){
				UP = 1;
			}
		}
	}
}

	if(posi_deuxieme_ia.y > terrain_y_ia-vision){
		DOWN = 1;
	}else{
		if(etat_deuxieme_ia == DOWN_2){
			for (int i = 1; i < 2; ++i){
				if(tableau_deplacement_ia[posi_deuxieme_ia.x][posi_deuxieme_ia.y+size_main_ia*i] != 0){
					DOWN = 1;
				}
			}
		}
	}

	if(posi_deuxieme_ia.x > terrain_x_ia-vision){
		RIGHT = 1;
	}else{
		if(etat_deuxieme_ia == RIGHT_2){
			for (int i = 1; i < 2; ++i){
				if(tableau_deplacement_ia[posi_deuxieme_ia.x+size_main_ia*i][posi_deuxieme_ia.y]  != 0){
					RIGHT = 1;
				}
			}
		}
	}

	if(posi_deuxieme_ia.x < vision){
		LEFT = 1;
	}else{
		if(etat_deuxieme_ia == LEFT_2){
		for (int i = 1; i < 2; ++i){
			if(tableau_deplacement_ia[posi_deuxieme_ia.x-size_main_ia*i][posi_deuxieme_ia.y]  != 0){
				LEFT = 1;
			}
		}
	}
}
	
	if(etat_ia == IN_GAME && etat_main_ia != PAUSE){
		if(UP == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = RIGHT_2;
				changement_direction_ia = 1;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = DOWN_2;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = DOWN_2;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = DOWN_2;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(DOWN == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = UP_2;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = RIGHT_2;
				changement_direction_ia = 2;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = DOWN_2;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = DOWN_2;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(LEFT == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = RIGHT_2;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = RIGHT_2;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = UP_2;
				changement_direction_ia = 4;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = RIGHT_2;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(RIGHT == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = LEFT_2;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = LEFT_2;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = LEFT_2;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = UP_2;
				changement_direction_ia = 3;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(LEFT == 1 && UP == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = RIGHT_2;
				changement_direction_ia = 1;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = DOWN_2;
				changement_direction_ia = 4;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = DOWN_2;
				changement_direction_ia = 4;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = RIGHT_2;
				changement_direction_ia = 1;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(RIGHT == 1 && UP == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = LEFT_2;
				changement_direction_ia = 1;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = DOWN_2;
				changement_direction_ia = 3;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = LEFT_2;
				changement_direction_ia = 1;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = DOWN_2;
				changement_direction_ia = 3;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(LEFT == 1 && DOWN == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = UP_2;
				changement_direction_ia = 4;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = RIGHT_2;
				changement_direction_ia = 2;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = UP_2;
				changement_direction_ia = 4;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = RIGHT_2;
				changement_direction_ia = 2;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(RIGHT == 1 && DOWN == 1 && timer_deplacement_ia == 0 && changement_direction_ia == 0){
			if(etat_deuxieme_ia == UP_2){
				etat_deuxieme_ia = UP_2;
				changement_direction_ia = 3;
			}else if(etat_deuxieme_ia == DOWN_2){
				etat_deuxieme_ia = LEFT_2;
				changement_direction_ia = 2;
			}else if(etat_deuxieme_ia == LEFT_2){
				etat_deuxieme_ia = LEFT_2;
				changement_direction_ia = 2;
			}else if(etat_deuxieme_ia == RIGHT_2){
				etat_deuxieme_ia = UP_2;
				changement_direction_ia = 4;
			}

			timer_deplacement_ia = (rand() % (3 - 1 + 1)) + 1;
		}else if(changement_direction_ia == 1 && timer_deplacement_ia == 0){
			etat_deuxieme_ia = DOWN_2;

			changement_direction_ia = 0;
		}else if(changement_direction_ia == 2 && timer_deplacement_ia == 0){
			etat_deuxieme_ia = UP_2;

			changement_direction_ia = 0;
		}else if(changement_direction_ia == 3 && timer_deplacement_ia == 0){
			etat_deuxieme_ia = LEFT_2;

			changement_direction_ia = 0;
		}else if(changement_direction_ia == 4 && timer_deplacement_ia == 0){
			etat_deuxieme_ia = RIGHT_2;

			changement_direction_ia = 0;
		}else if(UP == 0 && DOWN == 0 && LEFT == 0 && RIGHT == 0 && start_ia != 0 && timer_deplacement_ia == 0){
			etat_deuxieme_ia = etat_deuxieme_ia;
		}else if(start_ia == 0){
			choix_depart_ia();
			start_ia = 1;
		}
	}

	if(timer_deplacement_ia != 0){
		timer_deplacement_ia--;
	}
}

void choix_mode_ia(int mode_ia){
	if(mode_ia == 1){
		brain_classique_ia();
	}else if(mode_ia == 2){
		brain_ia();
	}else if(mode_ia == 3){
		brain_miroir_ia();
	}
}