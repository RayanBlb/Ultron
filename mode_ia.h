#ifndef MODE_IA_H_
#define MODE_IA_H_

typedef struct position_struct_mode_ia{
	int x;
	int y;
}position_mode_ia;

void dessin_menu_mode_ia(void);
void dessin_texte_mode_ia(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
void dessin_fond_mode_ia(void);
void set_mode_ia(void);
void input_mode_ia(int mode_difficulte);
void delay_game_mode_ia(void);
void fermeture_sdl_mode_ia(void);
void get_screensize_mode_ia(void);
void mode_ia(int mode_difficulte);
void switch_screen_mode_ia(int mode_mode_ia);
void reinitialisation_mode_ia(int mode_mode_ia);
void set_fond_menu_mode_ia(void);
void switch_screen_type_ia(int mode_difficulte,int type_ia);
int init_mode_ia(void);

#endif