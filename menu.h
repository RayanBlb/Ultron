#ifndef MENU_H_
#define MENU_H_

typedef struct position_struct_menu{
	int x;
	int y;
}position_menu;

int dessin_menu_menu(void);
int dessin_texte_menu(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
int dessin_fond_menu();
int set_menu(void);
int input_menu(void);
int delay_game_menu(void);
int fermeture_sdl_menu(void);
int get_screensize_menu(void);
int init_menu(void);
int menu(void);
int switch_screen_menu(char *mode);
int reinitialisation_menu(void);
int set_fond_menu_menu(void);

#endif