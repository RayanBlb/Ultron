#ifndef high_score_H_
#define high_score_H_

typedef struct position_struct_high_score{
	int x;
	int y;
}position_high_score;

int dessin_menu_high_score(void);
int dessin_texte_high_score(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
int dessin_fond_high_score();
int dessin_high_score_high_score(void);
int input_high_score(void);
int delay_game_high_score(void);
int fermeture_sdl_high_score(void);
int get_screensize_high_score(void);
int init_high_score(void);
int high_score(void);
int switch_screen_high_score(void);
int reinitialisation_high_score(void);
int set_fond_high_score(void);
int read_high_score(char *file_name);
int set_high_score();
int write_high_score(char *chemin);

#endif