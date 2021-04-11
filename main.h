#ifndef MAIN_H_
#define MAIN_H_

int dessin_menu_main(void);
int dessin_texte_main(char *texte, SDL_Color couleur,int coef_position,int size_font_x,int size_font_y);
int dessin_fond_main();
int set_main();
int input_main();
int delay_game_main();
int fermeture_sdl_main();
int get_screensize_main();
int init_main();


#endif