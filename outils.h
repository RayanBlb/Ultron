#ifndef OUTILS_H_
#define OUTILS_H_

typedef struct liste{
	int score;
	char *nom;
	struct liste *pNext;
}liste;

typedef struct listePosition{
	int x;
	int y;
	struct listePosition *pNext;
}listePosition;

typedef struct position_struct{
	int x;
	int y;
}position;

void free_liste_high_score_outils(liste **pFirst);
void read_file_high_score_outils(char *chemin,char *nom_high_score_survie,int score_survie,liste *pFirst);
void write_liste_high_score_outils(liste *pFirst,char *chemin);
void write_file_outils(char *nom, int score,char *chemin);
void sup_liste_high_score_outils(char *chemin);
void delay_game_outils(int etat_survie, int mode_de_jeu);
void fermeture_sdl_outils(SDL_Window *fenetrePrincipale_survie, SDL_Renderer *renduPrincipale_survie, Mix_Chunk *explosion_survie, SDL_Surface *main_surface_survie, SDL_Texture *main_texture_survie,SDL_Surface *deuxieme_surface_versus,SDL_Texture *deuxieme_texture_versus,TTF_Font* font_general);
void switch_screen_outils(SDL_Window *fenetrePrincipale_survie, SDL_Renderer *renduPrincipale_survie, Mix_Music *music_de_fond_survie, SDL_Surface *main_surface_survie, SDL_Texture *main_texture_survie, int etat_survie,SDL_Surface *deuxieme_surface_versus,SDL_Texture *deuxieme_texture_versus,TTF_Font* font_general);
void play_musique_outils(Mix_Music **music_de_fond_survie);
void play_explosion_outils(Mix_Chunk **explosion_survie);
void allocation_tableau_outils(int ***tableau_deplacement, int width_windows_survie, int height_windows_survie);
void free_tableau_outils(int ***tableau_deplacement, int width_windows_survie);
void dessin_personnage_outils(listePosition *pFirst,SDL_Renderer *renduPrincipale_survie,SDL_Texture *main_texture_survie,int size_main_survie);
void free_log_deplacement_outils(listePosition **pFirst);
void dessin_texte_outils(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y,TTF_Font *font_general,int width_windows, int height_windows,SDL_Renderer *renduPrincipale);
void dessin_fond_outils(int taille_carreaux,SDL_Renderer *renduPrincipale,int width_windows, int height_windows);
int tri_high_score_outils(liste **pFirst,char *nom,int score);int compte_nom_high_score_outils(char *nom_high_score_survie);
int compte_nom_high_score_outils(char *nom_high_score_survie);
int log_deplacement_outils(listePosition **pFirst,int position_x,int position_y);

#endif