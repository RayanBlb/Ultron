#ifndef OUTILS_H_
#define OUTILS_H_

typedef struct liste{
	int score;
	char *nom;
	struct liste *pNext;
}liste;

int free_liste_high_score_outils(liste **pFirst);
int read_file_high_score_outils(char *chemin,char *nom_high_score_survie,int score_survie,liste *pFirst);
int write_liste_high_score_outils(liste *pFirst,char *chemin);
int tri_high_score_outils(liste **pFirst,char *nom,int score);
int write_file_outils(char *nom, int score,char *chemin);
int sup_liste_high_score_outils(char *chemin);
int delay_game_outils(int etat_survie);
int fermeture_sdl_outils(SDL_Window *fenetrePrincipale_survie, SDL_Renderer *renduPrincipale_survie, Mix_Chunk *explosion_survie, SDL_Surface *main_surface_survie, SDL_Texture *main_texture_survie);
int switch_screen_outils(SDL_Window *fenetrePrincipale_survie, SDL_Renderer *renduPrincipale_survie, Mix_Music *music_de_fond_survie, SDL_Surface *main_surface_survie, SDL_Texture *main_texture_survie, int etat_survie);
int play_musique_outils(Mix_Music **music_de_fond_survie);
int play_explosion_outils(Mix_Chunk **explosion_survie);
int compte_nom_high_score_outils(char *nom_high_score_survie);
int allocation_tableau_outils(int ***tableau_deplacement, int width_windows_survie, int height_windows_survie);
int free_tableau_outils(int ***tableau_deplacement, int width_windows_survie);

#endif