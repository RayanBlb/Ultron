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

liste *pFirst_versus = NULL;

int etat_versus = START;//Etat de la partie global
int etat_main_versus = START;//etat du joueur 1 (blanc)
int etat_deuxieme_versus = START;//etat du joueur 2 (rouge)

int prev_etat_main_versus = START;
int prev_etat_deuxieme_versus = START;

int width_windows_versus = 0;
int height_windows_versus = 0;

int score_versus = 0;

int terrain_x_versus = 0;
int terrain_y_versus = 0;

int size_main_versus = 32;//correspond a la taille du personnage, mais également au carreaux du terrain

int **tableau_deplacement_versus;

char nom_high_score_versus[30];//correspond au nom a entrer dans les high score

position_versus posi_main_versus = {0,0};
position_versus posi_deuxieme_versus = {0,0};

SDL_Window *fenetrePrincipale_versus = NULL;
SDL_Renderer *renduPrincipale_versus = NULL;

SDL_Surface *main_surface_versus = NULL;
SDL_Texture *main_texture_versus = NULL;

SDL_Surface *deuxieme_surface_versus = NULL;
SDL_Texture *deuxieme_texture_versus = NULL;

SDL_Surface *background_score_surface_versus = NULL;
SDL_Texture *background_score_texture_versus = NULL;

SDL_Surface *background_high_score_surface_versus = NULL;
SDL_Texture *background_high_score_texture_versus = NULL;

TTF_Font *font_general_versus = NULL;

Mix_Music *music_de_fond_versus = NULL;
Mix_Chunk *explosion_versus = NULL;

//Boucle principale
void versus(int mode_difficulte){
	init_versus();//Initialisation de SDL et des autre bibliothéque 
	SDL_GetWindowSize(fenetrePrincipale_versus, &width_windows_versus, &height_windows_versus);//récuperation de la taille de la fenetre
	reinitialisation_versus(mode_difficulte);//Réinitialisation des variables globales
	allocation_tableau_outils(&tableau_deplacement_versus,width_windows_versus,height_windows_versus);//Allocation du tableau de déplacement
	initialisation_position_main_versus(mode_difficulte);//Initialisation position des joueurs
	play_musique_outils(&music_de_fond_versus);//Lancement de la musique
	set_start_versus();//Affichage des différents éléments du terrain
	while(etat_versus < GAME_OVER_1_win){
		input_versus();//input déplacement joueur 1 et 2
		update_versus();//actualisation des coordonnée de l'ia et du joueur
		set_versus();//actualisation terrain
		delay_game_outils(etat_versus,1);//gestion du taux de rafraîchissement
		//SDL_Log("1 - debug : etat_versus = %d , etat_main_versus = %d , etat_deuxieme_versus = %d\n 2 - debug : terrain_x_versus : %d , terrain_y_versus : %d \n",etat_versus,etat_main_versus,etat_deuxieme_versus,terrain_x_versus,terrain_y_versus);
	}
	set_game_over_versus();
}
/*------------------------------------------*/

//Fonction d'affichage en fonction de l'état de versus
void set_versus(){//Permet d'afficher les déplacement des joueurs
	if(etat_versus && etat_versus < GAME_OVER_1_win && (etat_main_versus != PAUSE || etat_deuxieme_versus != PAUSE)){
		dessin_main_versus();
		dessin_deuxieme_versus();
		dessin_background_score_versus();
		dessin_score_versus();
		SDL_RenderPresent(renduPrincipale_versus);
	}else if (etat_main_versus == PAUSE || etat_deuxieme_versus == PAUSE){
		dessin_background_score_versus();
		dessin_score_versus();
		dessin_pause_versus();
		SDL_RenderPresent(renduPrincipale_versus);
	}
}

void set_start_versus(){//permet d'initialiser le terrain ainsi que les joueurs
	if(etat_versus == START){
		dessin_fond_versus();
		dessin_background_score_versus();
		dessin_main_versus();
		dessin_deuxieme_versus();
		dessin_score_versus();
		SDL_RenderPresent(renduPrincipale_versus);
	}
}

void set_game_over_versus(){//Affichage du game over ainsi que de la possibilité de rentrer un pseudo pour le high score
	if(etat_versus >= GAME_OVER_1_win){
		dessin_fond_versus();
		dessin_background_score_versus();
	}
	while(1){
		dessin_background_high_score_versus();
		input_high_score_versus();
		dessin_high_score_versus();
		dessin_score_versus();
		dessin_game_over_versus();
		SDL_RenderPresent(renduPrincipale_versus);
		delay_game_outils(etat_versus,1);
	}
}
/*------------------------------------------*/

//Fonction de réinitialisation des variable global
void reinitialisation_versus(int mode_difficulte){//Réinitialisation des variables globale
	if(mode_difficulte == 1){//changement de la taille du terrain en fonction de la difficulté
		size_main_versus = 8;
	}else if(mode_difficulte == 2){
		size_main_versus = 16;
	}else if(mode_difficulte == 3){
		size_main_versus = 32;
	}

	etat_versus = START;
	etat_main_versus = START;
	etat_deuxieme_versus = START;

	score_versus = 0;

	if(tableau_deplacement_versus){
		free_tableau_outils(&tableau_deplacement_versus,width_windows_versus);
	}

	strcpy(nom_high_score_versus,"");

	int taille_score = 210/size_main_versus;

	terrain_x_versus = width_windows_versus - taille_score*size_main_versus;
	terrain_y_versus = height_windows_versus;
}

void initialisation_position_main_versus(int mode_difficulte){//Initialisation du spawn des joueurs en fonction du mode de jeu choisi
	srand(time(0));

	int y = (rand() % (20))*size_main_versus;//size main augmente ou diminue en fonction du niveau de difficulté
	int coef_coordonne = 0;//on multipliant plus bas par "coef_coordonne" cela permet d'avoir des coordonées qui couvre tous le terrain

	if(mode_difficulte == 1){//permet d'avoir des coordonnées aléatoire sur tout le terrain en fonction du mode de diffculté
		coef_coordonne = 4;
	}else if(mode_difficulte == 2){
		coef_coordonne = 2;
	}else if(mode_difficulte == 3){
		coef_coordonne = 1;
	}

	posi_main_versus.x = size_main_versus*4;
	posi_main_versus.y = y*coef_coordonne;

	posi_deuxieme_versus.x = terrain_x_versus-size_main_versus*5;
	posi_deuxieme_versus.y = y*coef_coordonne;

	tab_deplacement_versus(posi_main_versus.x,posi_main_versus.y,1);
	tab_deplacement_versus(posi_deuxieme_versus.x,posi_deuxieme_versus.y,2);
}
/*------------------------------------------*/

//Fonction qui vont permettre de dessiner les différents éléments à afficher
void dessin_main_versus(){//création du premier joueurs (joueur blanc)
	SDL_Rect dest = { posi_main_versus.x,posi_main_versus.y, size_main_versus, size_main_versus};
	SDL_RenderCopy(renduPrincipale_versus,main_texture_versus,NULL,&dest);
}

void dessin_deuxieme_versus(){//Création du deuxieme joueur (joueur rouge)
	SDL_Rect dest = { posi_deuxieme_versus.x,posi_deuxieme_versus.y, size_main_versus, size_main_versus};
	SDL_RenderCopy(renduPrincipale_versus,deuxieme_texture_versus,NULL,&dest);
}

void dessin_background_score_versus(){//création du background du score en haut a droite
	SDL_Rect dest = { terrain_x_versus+1, 0, width_windows_versus - terrain_x_versus , height_windows_versus};
	SDL_RenderCopy(renduPrincipale_versus,background_score_texture_versus,NULL,&dest);
}

void dessin_score_versus(){//création du score en haut a droite
	char score_texte[20];
	SDL_Color couleur_font = {255, 255, 255};

	sprintf(score_texte,"Score : %d", score_versus);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_versus,score_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_versus, score_surface);

	SDL_Rect dest = {terrain_x_versus+10,10,175,50};

	SDL_RenderCopy(renduPrincipale_versus, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

void dessin_game_over_versus(){//création du game over ainsi que du score
	char game_over_texte[43];
	SDL_Color couleur_font = {255, 255, 255};

	if(etat_versus == GAME_OVER_1_win){
		sprintf(game_over_texte,"WHITE WIN !!! SCORE FINAL : %d ", score_versus);
	}else if(etat_versus == GAME_OVER_2_win){
		sprintf(game_over_texte,"RED WIN !!! SCORE FINAL : %d ", score_versus);
	}else if(etat_versus == GAME_OVER_1_suicide){
		sprintf(game_over_texte,"RED WIN !!! SCORE FINAL : %d ", score_versus);
	}else if(etat_versus == GAME_OVER_2_suicide){
		sprintf(game_over_texte,"WHITE WIN !!! SCORE FINAL : %d ", score_versus);
	}

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_versus,game_over_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_versus, score_surface);

	int size_game_over_x = 325*4;
	int size_game_over_y = 50*4;

	int position_x = (terrain_x_versus - size_game_over_x)/2;
	int position_y = (terrain_y_versus - size_game_over_y*2)/2;
	
	SDL_Rect dest = {position_x,position_y,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale_versus, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

void dessin_pause_versus(){//création de la pause
	SDL_Color couleur_font = {255, 255, 255};

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_versus,"PAUSE", couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_versus, score_surface);

	SDL_Rect dest = {terrain_x_versus+10,(terrain_y_versus-50)/2,175,50};

	SDL_RenderCopy(renduPrincipale_versus, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

void dessin_fond_versus(){//création du fond quadrillé

	SDL_SetRenderDrawColor(renduPrincipale_versus,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale_versus);
	SDL_SetRenderDrawColor(renduPrincipale_versus,77, 77, 77, 255);

	for(int x = 0; x <= terrain_x_versus; x += size_main_versus){
		SDL_RenderDrawLine(renduPrincipale_versus, x, 0, x, height_windows_versus);
	}

	for(int y = 0; y < terrain_y_versus; y += size_main_versus){
		SDL_RenderDrawLine(renduPrincipale_versus, 0, y, terrain_x_versus, y);
	}
}

void dessin_high_score_versus(){//Affichage au fur et a mesure du nom du joueur lorsqu'il tape son nom
	char game_over_texte[43];
	SDL_Color couleur_font = {255, 255, 255};

	sprintf(game_over_texte,"%s", nom_high_score_versus);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general_versus,game_over_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale_versus, score_surface);

	int size_game_over_x = 100*compte_nom_high_score_outils(nom_high_score_versus);
	int size_game_over_y = 175;

	int position_x = (terrain_x_versus - size_game_over_x)/2;
	int position_y = (terrain_y_versus + size_game_over_y*0.5)/2;	
	
	SDL_Rect dest = {position_x,position_y,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale_versus, score_texture, NULL, &dest);

	SDL_DestroyTexture(score_texture);
	SDL_FreeSurface(score_surface);
}

void dessin_background_high_score_versus(){//Affichage du carré gris derriere le nom du joueur lorsqu'il tape son nom
	int size_game_over_x = 1100;
	int size_game_over_y = 175;

	int position_x = (terrain_x_versus - size_game_over_x)/2;
	int position_y = (terrain_y_versus + size_game_over_y*0.5)/2;

	SDL_Rect dest = {position_x, position_y, size_game_over_x, size_game_over_y};
	SDL_RenderCopy(renduPrincipale_versus,background_high_score_texture_versus,NULL,&dest);
}
/*------------------------------------------*/

//Fonction relative au input du joueur
void input_versus(){//Gestion des input de déplacement des joueurs
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_versus();
				break;
			case SDL_QUIT:
				fermeture_sdl_outils(fenetrePrincipale_versus,renduPrincipale_versus,explosion_versus,main_surface_versus,main_texture_versus,deuxieme_surface_versus,deuxieme_texture_versus,font_general_versus);
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_z && etat_versus < GAME_OVER_1_win){
					etat_main_versus = UP_1;

				}else if(touche.key.keysym.sym == SDLK_s && etat_versus < GAME_OVER_1_win){
					etat_main_versus = DOWN_1;

				}else if(touche.key.keysym.sym == SDLK_d && etat_versus < GAME_OVER_1_win){
					etat_main_versus = RIGHT_1;
					
				}else if(touche.key.keysym.sym == SDLK_q && etat_versus < GAME_OVER_1_win){
					etat_main_versus = LEFT_1;
					
				}else if(touche.key.keysym.sym == SDLK_UP && etat_versus < GAME_OVER_1_win){
					etat_deuxieme_versus = UP_2;

				}else if(touche.key.keysym.sym == SDLK_DOWN && etat_versus < GAME_OVER_1_win){
					etat_deuxieme_versus = DOWN_2;

				}else if(touche.key.keysym.sym == SDLK_RIGHT && etat_versus < GAME_OVER_1_win){
					etat_deuxieme_versus = RIGHT_2;
					
				}else if(touche.key.keysym.sym == SDLK_LEFT && etat_versus < GAME_OVER_1_win){
					etat_deuxieme_versus = LEFT_2;
					
				}else if(touche.key.keysym.sym == SDLK_SPACE && etat_versus < GAME_OVER_1_win){
					prev_etat_main_versus = etat_main_versus;
					prev_etat_deuxieme_versus = etat_deuxieme_versus;
					etat_main_versus = PAUSE;
					etat_deuxieme_versus = PAUSE;

				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_outils(fenetrePrincipale_versus,renduPrincipale_versus,music_de_fond_versus,main_surface_versus,main_texture_versus, etat_versus,deuxieme_surface_versus,deuxieme_texture_versus,font_general_versus);
				}

				if(etat_versus == START){
					etat_versus = IN_GAME;
					if(etat_deuxieme_versus == START)etat_deuxieme_versus = LEFT_2;
					if(etat_main_versus == START)etat_main_versus = RIGHT_1;
				}

				if(etat_main_versus == PAUSE && etat_deuxieme_versus != PAUSE){
					etat_main_versus = prev_etat_main_versus;
				}else if(etat_main_versus != PAUSE && etat_deuxieme_versus == PAUSE){
					etat_deuxieme_versus = prev_etat_deuxieme_versus;
				}
				
				break;
			}
		}
	}

void input_high_score_versus(){//gestion des input lorsque le joueur tape son nom
	SDL_Event touche;

	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input_menu();
				break;
			case SDL_QUIT:
				fermeture_sdl_outils(fenetrePrincipale_versus,renduPrincipale_versus,explosion_versus,main_surface_versus,main_texture_versus,deuxieme_surface_versus,deuxieme_texture_versus,font_general_versus);
				break;

			case SDL_KEYDOWN:
				if(touche.key.keysym.sym != SDLK_RGUI && touche.key.keysym.sym != SDLK_LGUI && touche.key.keysym.sym != SDLK_CAPSLOCK && touche.key.keysym.sym != SDLK_HOME && touche.key.keysym.sym != SDLK_PAGEDOWN && touche.key.keysym.sym != SDLK_PAGEUP && touche.key.keysym.sym != SDLK_END && touche.key.keysym.sym != SDLK_BACKSPACE && touche.key.keysym.sym != SDLK_DELETE && touche.key.keysym.sym != SDLK_INSERT && touche.key.keysym.sym != SDLK_TAB && touche.key.keysym.sym != SDLK_RIGHT && touche.key.keysym.sym != SDLK_RIGHT && touche.key.keysym.sym != SDLK_LEFT && touche.key.keysym.sym != SDLK_DOWN && touche.key.keysym.sym != SDLK_UP && touche.key.keysym.sym != SDLK_LCTRL && touche.key.keysym.sym != SDLK_RCTRL && touche.key.keysym.sym != SDLK_LALT && touche.key.keysym.sym != SDLK_RALT && touche.key.keysym.sym != SDLK_RSHIFT && touche.key.keysym.sym != SDLK_LSHIFT && touche.key.keysym.sym != SDLK_ESCAPE && touche.key.keysym.sym != SDLK_RETURN && touche.key.keysym.sym != SDLK_BACKSPACE && touche.key.keysym.sym != SDLK_LSHIFT && touche.key.keysym.sym != SDLK_RSHIFT && touche.key.keysym.sym != SDLK_SPACE  && compte_nom_high_score_outils(nom_high_score_versus) < 10){
					strcat(nom_high_score_versus,SDL_GetKeyName(touche.key.keysym.sym));
				}else if(touche.key.keysym.sym == SDLK_BACKSPACE && compte_nom_high_score_outils(nom_high_score_versus) > 0){
					int compteur = compte_nom_high_score_outils(nom_high_score_versus);
					compteur--;
					strcpy(&nom_high_score_versus[compteur],"");
				}else if(touche.key.keysym.sym == SDLK_RETURN){
					read_file_high_score_outils("./score_versus.txt",nom_high_score_versus,score_versus,pFirst_versus);
					switch_screen_outils(fenetrePrincipale_versus,renduPrincipale_versus,music_de_fond_versus,main_surface_versus,main_texture_versus,etat_versus,deuxieme_surface_versus,deuxieme_texture_versus,font_general_versus);
				}else if(touche.key.keysym.sym == SDLK_LSHIFT){
				}else if(touche.key.keysym.sym == SDLK_RSHIFT){
				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					switch_screen_outils(fenetrePrincipale_versus,renduPrincipale_versus,music_de_fond_versus,main_surface_versus,main_texture_versus, etat_versus,deuxieme_surface_versus,deuxieme_texture_versus,font_general_versus);
				}
				break;
			}
		}
	}
/*------------------------------------------*/

//Fonction gestion enregistrement de déplacement
void update_versus(){//modification des coordonnées des joueurs ainsi que augmentation du score

	switch(etat_main_versus){
		case UP_1:
			if(posi_main_versus.y<0){
				posi_main_versus.y = 0;
			}else if(posi_main_versus.y>0){
				posi_main_versus.y-=size_main_versus;
			}
			tab_deplacement_versus(posi_main_versus.x,posi_main_versus.y,1);
			break;
		case DOWN_1:
			if(posi_main_versus.y>terrain_y_versus-size_main_versus){
				posi_main_versus.y = terrain_y_versus-size_main_versus;
			}else if(posi_main_versus.y<terrain_y_versus-size_main_versus){
				posi_main_versus.y+=size_main_versus;
			}
			tab_deplacement_versus(posi_main_versus.x,posi_main_versus.y,1);
			break;
		case RIGHT_1:
			if(posi_main_versus.x>terrain_x_versus-size_main_versus){
				posi_main_versus.x = terrain_x_versus-size_main_versus;
			}else if(posi_main_versus.x<terrain_x_versus-size_main_versus){
				posi_main_versus.x+=size_main_versus;
			}
			tab_deplacement_versus(posi_main_versus.x,posi_main_versus.y,1);
			break;
		case LEFT_1:
			if(posi_main_versus.x<0){
				posi_main_versus.x = 0;
			}else if(posi_main_versus.x>0){
				posi_main_versus.x-=size_main_versus;
			}
			tab_deplacement_versus(posi_main_versus.x,posi_main_versus.y,1);
			break;
		default:
			break;
	}

	if(etat_versus < GAME_OVER_1_win){//permet de ne pas voir de déplacement du joueur 2 après le game over
		switch(etat_deuxieme_versus){
			case UP_2:
				if(posi_deuxieme_versus.y<0){
					posi_deuxieme_versus.y = 0;
				}else if(posi_deuxieme_versus.y>0){
					posi_deuxieme_versus.y-=size_main_versus;
				}
				tab_deplacement_versus(posi_deuxieme_versus.x,posi_deuxieme_versus.y,2);
				break;
			case DOWN_2:
				if(posi_deuxieme_versus.y>terrain_y_versus-size_main_versus){
					posi_deuxieme_versus.y = terrain_y_versus-size_main_versus;
				}else if(posi_deuxieme_versus.y<terrain_y_versus-size_main_versus){
					posi_deuxieme_versus.y+=size_main_versus;
				}
				tab_deplacement_versus(posi_deuxieme_versus.x,posi_deuxieme_versus.y,2);
				break;
			case RIGHT_2:
				if(posi_deuxieme_versus.x>terrain_x_versus-size_main_versus){
					posi_deuxieme_versus.x = terrain_x_versus-size_main_versus;
				}else if(posi_deuxieme_versus.x<terrain_x_versus-size_main_versus){
					posi_deuxieme_versus.x+=size_main_versus;
				}
				tab_deplacement_versus(posi_deuxieme_versus.x,posi_deuxieme_versus.y,2);
				break;
			case LEFT_2:
				if(posi_deuxieme_versus.x<0){
					posi_deuxieme_versus.x = 0;
				}else if(posi_deuxieme_versus.x>0){
					posi_deuxieme_versus.x-=size_main_versus;
				}
				tab_deplacement_versus(posi_deuxieme_versus.x,posi_deuxieme_versus.y,2);
				break;
			default:
				break;
			}
		}

	if((etat_deuxieme_versus != PAUSE || etat_main_versus != PAUSE) && (etat_deuxieme_versus != START || etat_main_versus != START) && etat_versus < GAME_OVER_1_win){
		score_versus++;
	}
}

int tab_deplacement_versus(int x, int y,int joueur){//permet d'ajouter les déplacement du joueur dans le tableau déplacement
	if(tableau_deplacement_versus[x][y] == 1 && joueur != 1){
		etat_versus = GAME_OVER_1_win;
		Mix_FreeMusic(music_de_fond_versus);
		play_explosion_outils(&explosion_versus);
		return 0;
	}else if(tableau_deplacement_versus[x][y] == 1 && joueur == 1){
		etat_versus = GAME_OVER_1_suicide;
		Mix_FreeMusic(music_de_fond_versus);
		play_explosion_outils(&explosion_versus);
		return 0;
	}else if(tableau_deplacement_versus[x][y] == 2 && joueur != 2){
		etat_versus = GAME_OVER_2_win;
		Mix_FreeMusic(music_de_fond_versus);
		play_explosion_outils(&explosion_versus);
		return 0;
	}else if(tableau_deplacement_versus[x][y] == 2 && joueur == 2){
		etat_versus = GAME_OVER_2_suicide;
		Mix_FreeMusic(music_de_fond_versus);
		play_explosion_outils(&explosion_versus);
		return 0;
	}else if(tableau_deplacement_versus[x][y] == 0 && joueur == 1){
		tableau_deplacement_versus[x][y] = 1;
		return 0;
	}else if(tableau_deplacement_versus[x][y] == 0 && joueur == 2){
		tableau_deplacement_versus[x][y] = 2;
		return 0;
	}

	return 0;
}

/*------------------------------------------*/

//Fonction initialisation
int init_versus(){//Fonction qui initialise SDL ainsi que c'est bibliothéque, donne un titre a la fenetre ainsi qu'un icon, viens initialisé la font ainsi que les différents sprite utiliser dans le mode ia

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

	if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_SHOWN, &fenetrePrincipale_versus, &renduPrincipale_versus) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale_versus);
    SDL_RenderPresent(renduPrincipale_versus);

    SDL_SetWindowTitle(fenetrePrincipale_versus, "Ultron");

    SDL_Surface* icon_programme_versus = SDL_LoadBMP("./Sprites/icon.bmp");
	SDL_SetWindowIcon(fenetrePrincipale_versus, icon_programme_versus);

	main_surface_versus = SDL_LoadBMP("./Sprites/main.bmp");
	main_texture_versus = SDL_CreateTextureFromSurface(renduPrincipale_versus,main_surface_versus);

	deuxieme_surface_versus = SDL_LoadBMP("./Sprites/deuxieme.bmp");
	deuxieme_texture_versus = SDL_CreateTextureFromSurface(renduPrincipale_versus,deuxieme_surface_versus);

	background_score_surface_versus = SDL_LoadBMP("./Sprites/background_score.bmp");
	background_score_texture_versus = SDL_CreateTextureFromSurface(renduPrincipale_versus,background_score_surface_versus);

	background_high_score_surface_versus = SDL_LoadBMP("./Sprites/background_high_score.bmp");
	background_high_score_texture_versus = SDL_CreateTextureFromSurface(renduPrincipale_versus,background_high_score_surface_versus);

	font_general_versus = TTF_OpenFont("./Font/font.ttf", 16);

	return 0;
}
/*------------------------------------------*/