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

#define START_SURVIE 1
#define START_VERSUS 2
#define START_IA 3
#define OPTION 4
#define HIGH_SCORE 5
#define QUITTER 6

int etat_menu = START_SURVIE;
int mode_difficulte_menu = 2;

int width_windows_menu = 0;
int height_windows_menu = 0;

SDL_Color couleur_font_menu = {255, 255, 255};

SDL_Window* fenetrePrincipale_menu = NULL;
SDL_Renderer* renduPrincipale_menu = NULL;

TTF_Font* font_general_menu = NULL;

//Boucle principale
void menu(int mode_difficulte){
	reinitialisation_menu(mode_difficulte);//Réinitialisation des variables globales
	init_menu();//Initialisation de SDL et des autre bibliothéque 
	get_screensize_menu();//Récuperation de la taille de la fenetre
	set_fond_menu_menu();//Affichage du fond, du titre ainsi que du menu
	while(1){
		input_menu();//affichage du menu en fonction de l'input du joueur
		delay_game_menu();//Taux de rafraîchissement de 60 fps
		//SDL_Log("etat : %d ",etat_menu);
	}
}
/*------------------------------------------*/

//Fonction de réinitialisation des variable global
void reinitialisation_menu(int mode_difficulte){//Viens changer la varible globale de difficulté en fonction de la variable placer en paramètre de la fonction menu
	if(mode_difficulte == 1){
		mode_difficulte_menu = 1;
	}else if(mode_difficulte == 2){
		mode_difficulte_menu = 2;
	}else if(mode_difficulte == 3){
		mode_difficulte_menu = 3;
	}
	etat_menu = START_SURVIE;
}
/*------------------------------------------*/

//Fonction d'affichage du menu
void set_menu(){
	dessin_fond_outils(32,renduPrincipale_menu,width_windows_menu,height_windows_menu);//création fond quadrillé
	dessin_menu_menu();//Affiche le menu en fonction de l'état qui a été changer par le joueur
	SDL_RenderPresent(renduPrincipale_menu);
}

void set_fond_menu_menu(){//Fonction affichage du menu
	dessin_fond_outils(32,renduPrincipale_menu,width_windows_menu,height_windows_menu);//création fond quadrillé
	dessin_menu_menu();//Affichage du menu
	SDL_RenderPresent(renduPrincipale_menu);
}
/*------------------------------------------*/

//Fonction qui vont permettre de dessiner les différents éléments à afficher
void dessin_menu_menu(){//Affiche du menu en fonction de l'onglet selectionner
	SDL_Color couleur_font_selection = {255, 0, 0};

	dessin_texte_outils("ULTRON",couleur_font_menu,-2,600,175,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);//Affichage du titre

	if(etat_menu == START_SURVIE){
		dessin_texte_outils("Survie",couleur_font_selection,-2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("1 VS 1",couleur_font_menu,-1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("VS IA",couleur_font_menu,0,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Difficulte",couleur_font_menu,1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("High Score",couleur_font_menu,2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Quitter",couleur_font_menu,3,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
	}else if(etat_menu == START_VERSUS ){
		dessin_texte_outils("Survie",couleur_font_menu,-2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("1 VS 1",couleur_font_selection,-1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("VS IA",couleur_font_menu,0,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Difficulte",couleur_font_menu,1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("High Score",couleur_font_menu,2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Quitter",couleur_font_menu,3,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
	}else if(etat_menu == START_IA ){
		dessin_texte_outils("Survie",couleur_font_menu,-2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("1 VS 1",couleur_font_menu,-1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("VS IA",couleur_font_selection,0,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Difficulte",couleur_font_menu,1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("High Score",couleur_font_menu,2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Quitter",couleur_font_menu,3,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
	}else if(etat_menu == OPTION ){
		dessin_texte_outils("Survie",couleur_font_menu,-2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("1 VS 1",couleur_font_menu,-1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("VS IA",couleur_font_menu,0,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Difficulte",couleur_font_selection,1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("High Score",couleur_font_menu,2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Quitter",couleur_font_menu,3,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
	}else if(etat_menu == HIGH_SCORE ){
		dessin_texte_outils("Survie",couleur_font_menu,-2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("1 VS 1",couleur_font_menu,-1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("VS IA",couleur_font_menu,0,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Difficulte",couleur_font_menu,1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("High Score",couleur_font_selection,2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Quitter",couleur_font_menu,3,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
	}else if(etat_menu == QUITTER ){
		dessin_texte_outils("Survie",couleur_font_menu,-2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("1 VS 1",couleur_font_menu,-1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("VS IA",couleur_font_menu,0,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Difficulte",couleur_font_menu,1,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("High Score",couleur_font_menu,2,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);
		dessin_texte_outils("Quitter",couleur_font_selection,3,400,100,font_general_menu,width_windows_menu,height_windows_menu,renduPrincipale_menu);	
	}
}
/*------------------------------------------*/

//Fonction relative au input du joueur
void input_menu(){//Permet de naviger dans le menu via les fleches, permet également de lancer en onglet en appuyant sur entrer
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
						switch_screen_menu("survie");
					}else if(etat_menu == START_VERSUS){
						switch_screen_menu("versus");
					}else if(etat_menu == OPTION){
						switch_screen_menu("difficulte");
					}else if(etat_menu == HIGH_SCORE){
						switch_screen_menu("high_score");
					}else if(etat_menu == START_IA){
						switch_screen_menu("ia");
					}else if(etat_menu == QUITTER){
						fermeture_sdl_menu();
					}
				}
				break;
			}
		}
	}
/*------------------------------------------*/

//Fonction initialisation
int init_menu(){//Fonction qui initialise SDL ainsi que c'est bibliothéque, donne un titre a la fenetre ainsi qu'un icon, viens initialisé la font

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

    SDL_Surface* icon_programme_survie = SDL_LoadBMP("../Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_menu, icon_programme_survie);

    font_general_menu = TTF_OpenFont("../Font/font.ttf", 16);

    return 0;
}
/*------------------------------------------*/

//Fonction qui vont permettre de gérer le delay, la fermetture et changement de fenetre ou encore de récuperer la taille de la fenetre afficher
void delay_game_menu(){//Permet de gerer le taux de réfréchissement de l'écran
	int lastTicks = 0;
	int maxFPS = 60;
	int delay = 0;

	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

void get_screensize_menu(){//Récuperer la taille de la fenetre
	SDL_GetWindowSize(fenetrePrincipale_menu, &width_windows_menu, &height_windows_menu);
}

void fermeture_sdl_menu(){//Permet de fermet la fenetre
	SDL_DestroyRenderer(renduPrincipale_menu);
	SDL_DestroyWindow(fenetrePrincipale_menu);
	TTF_CloseFont(font_general_menu);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void switch_screen_menu(char *mode){//permet de changer de fenetre en fonction du mode passer en paramètre
	SDL_DestroyRenderer(renduPrincipale_menu);
	SDL_DestroyWindow(fenetrePrincipale_menu);
	TTF_CloseFont(font_general_menu);
	if(strcmp(mode,"survie") == 0){
		survie(mode_difficulte_menu);
	}else if(strcmp(mode,"high_score") == 0){
		high_score();
	}else if(strcmp(mode,"versus") == 0){
		versus(mode_difficulte_menu);
	}else if(strcmp(mode,"difficulte") == 0){
		difficulte(mode_difficulte_menu);
	}else if(strcmp(mode,"ia") == 0){
		mode_ia(mode_difficulte_menu);
	}
}
/*------------------------------------------*/