#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

typedef struct position_struct{
	int x;
	int y;
}position;

int lastTicks = 0;
int maxFPS = 15;
int delay = 0;

int dir = 0;
int size_main = 32;

int width_windows = 0;
int height_windows = 0;

int tableau_deplacement;

int score = 0;
int taille_score = 6;

int terrain_x = 0;
int terrain_y = 0;

char score_texte[20];
char game_over_texte[43];

position posi_main = {0,0};

SDL_Event touche;
SDL_Color couleur_font = {255, 255, 255};

SDL_Window* fenetrePrincipale = NULL;
SDL_Renderer* renduPrincipale = NULL;

SDL_Surface* main_surface = NULL;
SDL_Texture* main_texture = NULL;

SDL_Surface* background_score_surface = NULL;
SDL_Texture* background_score_texture = NULL;

TTF_Font* font_general = NULL;

Mix_Music* music_de_fond = NULL;
Mix_Chunk* explosion = NULL;

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
int dessin_background_score(void);
int dessin_score(void);
int dessin_game_over(void);
int play_musique(void);
int play_explosion(void);

int main(int argc, char *argv[]){
	init();
	get_screensize();
	set();
	play_musique();
	while(1){
		input();
		update();
		set();
		delay_game();
		SDL_Log("1 - debug : dir = %d , Position x = %d , position y = %d \n 2 - debug : TICKS : %d, DELAY : %d \n 3 - debug : width_windows : %d height_windows : %d \n 4 - debug : terrain_x : %d terrain_y : %d",dir, posi_main.x, posi_main.y,lastTicks,delay,width_windows,height_windows,terrain_x,terrain_y);
	}
	return 0;
}

int dessin_tete(){
	SDL_Rect dest = { posi_main.x,posi_main.y, main_surface->w, main_surface->h};
	SDL_RenderCopy(renduPrincipale,main_texture,NULL,&dest);
	return 0;
}

int dessin_background_score(){
	SDL_Rect dest = { terrain_x+1, 0, width_windows - terrain_x , height_windows};
	SDL_RenderCopy(renduPrincipale,background_score_texture,NULL,&dest);
	return 0;
}

int dessin_score(){
	sprintf(score_texte,"Score : %d", score);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general,score_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale, score_surface);

	SDL_Rect dest = {terrain_x+10,10,175,50};

	SDL_RenderCopy(renduPrincipale, score_texture, NULL, &dest);
}

int dessin_game_over(){
	sprintf(game_over_texte,"GAME OVER !!! SCORE FINAL : %d ", score);

	SDL_Surface* score_surface = TTF_RenderText_Solid(font_general,game_over_texte, couleur_font);
	SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renduPrincipale, score_surface);

	int size_game_over_x = 325*4;
	int size_game_over_y = 50*4;
	
	SDL_Rect dest = {(terrain_x - size_game_over_x)/2,(terrain_y - size_game_over_y*1.5)/2 ,size_game_over_x,size_game_over_y};

	SDL_RenderCopy(renduPrincipale, score_texture, NULL, &dest);
}

int dessin_fond(){
	SDL_SetRenderDrawColor(renduPrincipale,22, 22, 22, 255);
	SDL_RenderClear(renduPrincipale);
	SDL_SetRenderDrawColor(renduPrincipale,77, 77, 77, 255);

	terrain_x = width_windows - taille_score*size_main;
	terrain_y = height_windows;

	for(int x = 0; x <= terrain_x; x += size_main){
		SDL_RenderDrawLine(renduPrincipale, x, 0, x, height_windows);
	}

	for(int y = 0; y < terrain_y; y += size_main){
		SDL_RenderDrawLine(renduPrincipale, 0, y, terrain_x, y);
	}

	SDL_RenderPresent(renduPrincipale);
	return 0;
}

int set(){
	if(dir && dir !=6){
		dessin_tete();
		dessin_background_score();
		dessin_score();
		SDL_RenderPresent(renduPrincipale);
	}else if(dir == 6){
		dessin_fond();
		dessin_background_score();
		dessin_score();
		dessin_game_over();
		SDL_RenderPresent(renduPrincipale);
	}else{
		dessin_fond();
		dessin_background_score();
		dessin_tete();
		dessin_score();
		SDL_RenderPresent(renduPrincipale);
	}
}

int init(){

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

	background_score_surface = SDL_LoadBMP("./Sprites/background_score.bmp");
	background_score_texture = SDL_CreateTextureFromSurface(renduPrincipale,background_score_surface);

	font_general = TTF_OpenFont("./Font/font.ttf", 16);
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
				if (touche.key.keysym.sym == SDLK_z && dir !=6){
					dir = 1;

				}else if(touche.key.keysym.sym == SDLK_s && dir !=6){
					dir = 2;

				}else if(touche.key.keysym.sym == SDLK_d && dir !=6){
					dir = 3;
					
				}else if(touche.key.keysym.sym == SDLK_q && dir !=6){
					dir = 4;
					
				}else if(touche.key.keysym.sym == SDLK_SPACE && dir !=6){
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
			if(posi_main.y>terrain_y-size_main){
				posi_main.y = terrain_y-size_main;
			}else if(posi_main.y<terrain_y-size_main){
				posi_main.y+=size_main;
			}
			tab_deplacement(posi_main.x,posi_main.y);
			break;
		case 3:
			if(posi_main.x>terrain_x-size_main){
				posi_main.x = terrain_x-size_main;
			}else if(posi_main.x<terrain_x-size_main){
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
		default:
			break;
	}
	if(dir != 5){
		if(dir != 0){
			if (dir != 6){
				score++;
			}
		}
	}

	return 0;
}

int tab_deplacement(int x, int y){
	int tableau_deplacement[width_windows][height_windows];

	if(tableau_deplacement[x][y] == 1){
		dir = 6;
		Mix_FreeMusic(music_de_fond);
		play_explosion();
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
	Mix_FreeChunk(explosion);
	SDL_Quit();
	return 0;
}

int get_screensize(){
	SDL_GetWindowSize(fenetrePrincipale, &width_windows, &height_windows);
	return 0;
}

int play_musique(){
	music_de_fond = Mix_LoadMUS("../../Documents/music.mp3");
	Mix_PlayMusic(music_de_fond, -1);
}

int play_explosion(){
	explosion = Mix_LoadWAV("../../Documents/explosion.wav");
	Mix_PlayChannel(-1, explosion, 0);
}