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
	reinitialisation_mode_ia(mode_difficulte);//Réinitialisation des variables globales
	init_mode_ia();//Initialisation de SDL et des autre bibliothéque 
	get_screensize_mode_ia();//Récuperation de la taille de la fenetre
	set_fond_menu_mode_ia();//Affichage du menu mode ai + fond + titre
	while(1){
		input_mode_ia(mode_difficulte);//Changement d'état du menu en fonction des input
		delay_game_mode_ia();//Taux de rafraîchissement de 60 fps
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
void set_mode_ia(){//Viens modifier l'affichage du menu en fonction des input du joueur
	dessin_fond_outils(32,renduPrincipale_mode_ia,width_windows_mode_ia,height_windows_mode_ia);
	dessin_menu_mode_ia();
	SDL_RenderPresent(renduPrincipale_mode_ia);
}

void set_fond_menu_mode_ia(){//affichage du fond + titre + menu
	dessin_fond_outils(32,renduPrincipale_mode_ia,width_windows_mode_ia,height_windows_mode_ia);
	dessin_menu_mode_ia();
	SDL_RenderPresent(renduPrincipale_mode_ia);
}
/*------------------------------------------*/

//Fonction qui vont permettre de dessiner les différents éléments à afficher
void dessin_menu_mode_ia(){//Création du menu en fonction de la selection du joueur
	SDL_Color couleur_font_selection = {255, 0, 0};

	dessin_texte_outils("ULTRON",couleur_font_mode_ia,-2,600,175,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);

	if(etat_mode_ia == CLASSIQUE){
		dessin_texte_outils("CLASSIQUE",couleur_font_selection,-1,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
		dessin_texte_outils("DE HAUTE EN BAS",couleur_font_mode_ia,0,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
		dessin_texte_outils("MIROIR",couleur_font_mode_ia,1,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
	}else if(etat_mode_ia == DE_HAUTE_EN_BAS ){
		dessin_texte_outils("CLASSIQUE",couleur_font_mode_ia,-1,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
		dessin_texte_outils("DE HAUTE EN BAS",couleur_font_selection,0,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
		dessin_texte_outils("MIROIR",couleur_font_mode_ia,1,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
	}else if(etat_mode_ia == MIRROIR ){
		dessin_texte_outils("CLASSIQUE",couleur_font_mode_ia,-1,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
		dessin_texte_outils("DE HAUTE EN BAS",couleur_font_mode_ia,0,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
		dessin_texte_outils("MIROIR",couleur_font_selection,1,400,100,font_general_mode_ia,width_windows_mode_ia,height_windows_mode_ia,renduPrincipale_mode_ia);
	}
}
/*------------------------------------------*/

//Fonction relative au input du joueur
void input_mode_ia(int mode_difficulte){//Fonction qui gérer la monter et descente dans le menu, ainsi que le lancement de l'ai choisi par le joueur
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
int init_mode_ia(){//Fonction qui initialise SDL ainsi que c'est bibliothéque, donne un titre a la fenetre ainsi qu'un icon, viens initialisé la font

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

    SDL_Surface* icon_programme_survie = SDL_LoadBMP("../Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_mode_ia, icon_programme_survie);

    font_general_mode_ia = TTF_OpenFont("../Font/font.ttf", 16);
    
    return 0;
}
/*------------------------------------------*/

//Fonction qui vont permettre de gérer le delay, la fermetture et changement de fenetre ou encore de récuperer la taille de la fenetre afficher
void delay_game_mode_ia(){//Permet de gerer le taux de réfréchissement de l'écran
	int lastTicks = 0;
	int maxFPS = 60;
	int delay = 0;

	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

void get_screensize_mode_ia(){//Récuperer la taille de la fenetre
	SDL_GetWindowSize(fenetrePrincipale_mode_ia, &width_windows_mode_ia, &height_windows_mode_ia);
}

void fermeture_sdl_mode_ia(){//Permet de fermet la fenetre
	SDL_DestroyRenderer(renduPrincipale_mode_ia);
	SDL_DestroyWindow(fenetrePrincipale_mode_ia);
	TTF_CloseFont(font_general_mode_ia);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}

void switch_screen_type_ia(int mode_difficulte,int type_ia){//Permet de lancer le mode d'ia choisi
	SDL_DestroyRenderer(renduPrincipale_mode_ia);
	SDL_DestroyWindow(fenetrePrincipale_mode_ia);
	TTF_CloseFont(font_general_mode_ia);
	ia(mode_difficulte,type_ia);
}

void switch_screen_mode_ia(int mode_difficulte){//Permet de revenir au menu principale
	SDL_DestroyRenderer(renduPrincipale_mode_ia);
	SDL_DestroyWindow(fenetrePrincipale_mode_ia);
	TTF_CloseFont(font_general_mode_ia);
	menu(mode_difficulte);
}
/*------------------------------------------*/