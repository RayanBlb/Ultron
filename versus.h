#ifndef VERSUS_H_
#define VERSUS_H_

typedef struct position_struct_versus{
	int x;
	int y;
}position_versus;

int init_versus(void);
int input_versus(void);
int update_versus(void);
int set_versus(void);
int dessin_main_versus(void);
int dessin_deuxieme_versus(void);
int dessin_fond_versus(void);
int get_screensize_versus(void);
int tab_deplacement_versus(int x, int y,int joueur);
int dessin_background_score_versus(void);
int dessin_score_versus(void);
int dessin_game_over_versus(void);
int versus(int mode);
int reinitialisation_versus(int mode);
int dessin_pause_versus(void);
int set_start_versus(void);
int set_game_over_versus(void);
int input_high_score_versus(void);
int dessin_high_score_versus(void);
int set_high_score_versus(void);
int dessin_background_high_score_versus(void);
int write_high_score_versus(void);
int initialisation_position_main_versus(void);

#endif