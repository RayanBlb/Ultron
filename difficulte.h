#ifndef DIFFICULTE_H_
#define DIFFICULTE_H_

typedef struct position_struct_difficulte{
	int x;
	int y;
}position_difficulte;

int dessin_menu_difficulte(void);
int dessin_texte_difficulte(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
int dessin_fond_difficulte();
int set_difficulte(void);
int input_difficulte(void);
int delay_game_difficulte(void);
int fermeture_sdl_difficulte(void);
int get_screensize_difficulte(void);
int init_difficulte(void);
int difficulte(int mode_difficulte);
int switch_screen_difficulte(int mode_difficulte);
int reinitialisation_difficulte(int mode_difficulte);
int set_fond_menu_difficulte(void);

#endif