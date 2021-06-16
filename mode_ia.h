#ifndef MODE_IA_H_
#define MODE_IA_H_

typedef struct position_struct_mode_ia{
	int x;
	int y;
}position_mode_ia;

int dessin_menu_mode_ia(void);
int dessin_texte_mode_ia(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
int dessin_fond_mode_ia();
int set_mode_ia(void);
int input_mode_ia(int mode_difficulte);
int delay_game_mode_ia(void);
int fermeture_sdl_mode_ia(void);
int get_screensize_mode_ia(void);
int init_mode_ia(void);
int mode_ia(int mode_difficulte);
int switch_screen_mode_ia(int mode_mode_ia);
int reinitialisation_mode_ia(int mode_mode_ia);
int set_fond_menu_mode_ia(void);
int switch_screen_type_ia(int mode_difficulte,int type_ia);

#endif