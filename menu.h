#ifndef MENU_H_
#define MENU_H_

typedef struct position_struct_menu{
	int x;
	int y;
}position_menu;

void dessin_menu_menu(void);
void dessin_texte_menu(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
void dessin_fond_menu();
void set_menu(void);
void input_menu(void);
void delay_game_menu(void);
void fermeture_sdl_menu(void);
void get_screensize_menu(void);
void menu(int mode_difficulte);
void switch_screen_menu(char *mode);
void reinitialisation_menu(int mode_difficulte);
void set_fond_menu_menu(void);
int init_menu(void);

#endif