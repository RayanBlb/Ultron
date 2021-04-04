#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

typedef struct position_struct{
	int x;
	int y;
}position;

int lastTicks;
int maxFPS = 15;
int delay = 0;

int dir = 0;
int size_main = 32;

int width_windows;
int height_windows;

int tableau_deplacement;

int score = 0;
int taille_score = 6;

position posi_main = {0,0};
position posi_fond = {0,0};

SDL_Event touche;

SDL_Window* fenetrePrincipale = NULL;
SDL_Renderer* renduPrincipale = NULL;

SDL_Surface* main_surface = NULL;
SDL_Texture* main_texture = NULL;

int init(void);
int input(void);
int update(void);
int set(void);
int delay_game(void);
int dessin_tete(void);
int dessin_fond(void);
int get_screensize(void);
int tab_deplacement(int x, int y);
int fermeture_sdl(void);

int main(int argc, char *argv[]){
	init();
	get_screensize();
	set();
	while(1){
		input();
		update();
		set();
		delay_game();
		SDL_Log("1 - debug : dir = %d , Position x = %d , position y = %d \n 2 - debug : TICKS : %d, DELAY : %d \n 3 - debug : width_windows : %d height_windows : %d",dir, posi_main.x, posi_main.y,lastTicks,delay,width_windows,height_windows);
	}
	return 0;
}

int dessin_tete(){
	SDL_Rect dest = { posi_main.x,posi_main.y, main_surface->w, main_surface->h};
	SDL_RenderCopy(renduPrincipale,main_texture,NULL,&dest);
	return 0;
}

int dessin_fond(){
	SDL_SetRenderDrawColor(renduPrincipale,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale);
	SDL_SetRenderDrawColor(renduPrincipale,55, 55, 55, 255);

	for(int x = 0; x <= ((width_windows/size_main)* size_main) - taille_score*size_main; x += size_main){
		SDL_RenderDrawLine(renduPrincipale, x, 0, x, height_windows);
	}

	for(int y = 0; y < ((height_windows/size_main) * size_main) + size_main; y += size_main){
		SDL_RenderDrawLine(renduPrincipale, 0, y, width_windows-taille_score*size_main, y);
	}

	SDL_RenderPresent(renduPrincipale);
	return 0;
}

int set(){
	if(dir){
		dessin_tete();
		SDL_RenderPresent(renduPrincipale);
	}else{
		dessin_fond();
		dessin_tete();
		SDL_RenderPresent(renduPrincipale);
	}
}

int input(){
	if(SDL_PollEvent(&touche)){
		switch(touche.type){
			case SDL_MOUSEMOTION:
				input();
				break;
			case SDL_QUIT:
				fermeture_sdl();
				exit(EXIT_SUCCESS);
				break;

			case SDL_KEYDOWN:
				if (touche.key.keysym.sym == SDLK_z){
					dir = 1;

				}else if(touche.key.keysym.sym == SDLK_s){
					dir = 2;

				}else if(touche.key.keysym.sym == SDLK_d){
					dir = 3;
					
				}else if(touche.key.keysym.sym == SDLK_q){
					dir = 4;
					
				}else if(touche.key.keysym.sym == SDLK_SPACE){
					dir = 5;

				}else if(touche.key.keysym.sym == SDLK_ESCAPE){
					fermeture_sdl();
					exit(EXIT_SUCCESS);
				}
				break;
			}
		}
	}


int update(){
	switch(dir){
		case 1:
			if(posi_main.y<0){
				posi_main.y = 0;
			}else if(posi_main.y>0){
				posi_main.y-=size_main;
			}
			tab_deplacement(posi_main.x,posi_main.y);
			break;
		case 2:
			if(posi_main.y>height_windows-32){
				posi_main.y = height_windows-32;
			}else if(posi_main.y<height_windows-32){
				posi_main.y+=size_main;
			}
			tab_deplacement(posi_main.x,posi_main.y);
			break;
		case 3:
			if(posi_main.x>width_windows-32){
				posi_main.x = width_windows-32;
			}else if(posi_main.x<width_windows-32){
				posi_main.x+=size_main;
			}
			tab_deplacement(posi_main.x,posi_main.y);
			break;
		case 4:
			if(posi_main.x<0){
				posi_main.x = 0;
			}else if(posi_main.x>0){
				posi_main.x-=size_main;
			}
			tab_deplacement(posi_main.x,posi_main.y);
			break;
		case 5:
			break;
	}
	score++;
	return 0;
}

int tab_deplacement(int x, int y){
	int tableau_deplacement[width_windows][height_windows];

	if(tableau_deplacement[x][y] == 1){
		fermeture_sdl();
		SDL_Log("Score : %d \n game over !!! \n",score);
		exit(EXIT_SUCCESS);
	}else if(tableau_deplacement[x][y] == 0){
		tableau_deplacement[x][y] = 1;
	}
	return 0;
}

int delay_game(){
	lastTicks = SDL_GetTicks();
	delay = 1000/maxFPS-SDL_GetTicks()+lastTicks;
	if(delay>0){
		SDL_Delay(delay);
	}
}

int fermeture_sdl(){
	SDL_DestroyRenderer(renduPrincipale);
	SDL_DestroyWindow(fenetrePrincipale);
	SDL_Quit();
	return 0;
}

int get_screensize(){
	SDL_GetWindowSize(fenetrePrincipale, &width_windows, &height_windows);
	return 0;
}

int init(){
	if(SDL_Init(SDL_INIT_VIDEO < 0)){
		printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
		return EXIT_FAILURE;
	}
	atexit(SDL_Quit);

	if(SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_SHOWN, &fenetrePrincipale, &renduPrincipale) < 0){
		printf("Erreur création fenetre : %s",SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

    SDL_RenderClear(renduPrincipale);
    SDL_RenderPresent(renduPrincipale);

    SDL_SetWindowTitle(fenetrePrincipale, "Ultron");

	main_surface = SDL_LoadBMP("./Sprites/main.bmp");
	main_texture = SDL_CreateTextureFromSurface(renduPrincipale,main_surface);
}