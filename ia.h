#ifndef IA_H_
#define IA_H_

typedef struct position_struct_ia{
	int x;
	int y;
}position_ia;

int init_ia(void);
int input_ia(void);
int update_ia(void);
int set_ia(void);
int dessin_main_ia(void);
int dessin_deuxieme_ia(void);
int dessin_fond_ia(void);
int get_screensize_ia(void);
int tab_deplacement_ia(int x, int y,int joueur);
int dessin_background_score_ia(void);
int dessin_score_ia(void);
int dessin_game_over_ia(void);
int ia(int mode);
int reinitialisation_ia(int mode);
int dessin_pause_ia(void);
int set_start_ia(void);
int set_game_over_ia(void);
int input_high_score_ia(void);
int dessin_high_score_ia(void);
int set_high_score_ia(void);
int dessin_background_high_score_ia(void);
int write_high_score_ia(void);
int initialisation_position_main_ia(void);

#endif