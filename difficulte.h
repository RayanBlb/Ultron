#ifndef DIFFICULTE_H_
#define DIFFICULTE_H_

typedef struct position_struct_difficulte{
	int x;
	int y;
}position_difficulte;

void dessin_menu_difficulte(void);
void dessin_texte_difficulte(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
void dessin_fond_difficulte(void);
void set_difficulte(void);
void input_difficulte(void);
void delay_game_difficulte(void);
void fermeture_sdl_difficulte(void);
void get_screensize_difficulte(void);
int init_difficulte(void);
void difficulte(int mode_difficulte);
void switch_screen_difficulte(int mode_difficulte);
void reinitialisation_difficulte(int mode_difficulte);
void set_fond_menu_difficulte(void);

#endif