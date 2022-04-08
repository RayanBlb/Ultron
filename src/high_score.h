#ifndef high_score_H_
#define high_score_H_

typedef struct position_struct_high_score{
	int x;
	int y;
}position_high_score;

void dessin_menu_high_score(void);
void dessin_high_score_high_score(void);
void input_high_score(void);
void delay_game_high_score(void);
void fermeture_sdl_high_score(void);
void get_screensize_high_score(void);
void high_score(void);
void switch_screen_high_score(void);
void reinitialisation_high_score(void);
void set_fond_high_score(void);
void read_high_score(char *file_name);
void set_high_score(void);
void write_high_score(char *chemin);
int init_high_score(void);

#endif