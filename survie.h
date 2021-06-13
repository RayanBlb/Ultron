#ifndef SURVIE_H_
#define SURVIE_H_

typedef struct position_struct_survie{
	int x;
	int y;
}position_survie;

int init_survie(void);
int input_survie(void);
int update_survie(void);
int set_survie(void);
int dessin_main_survie(void);
int dessin_fond_survie(void);
int get_screensize_survie(void);
int tab_deplacement_survie(int x, int y);
int dessin_background_score_survie(void);
int dessin_score_survie(void);
int dessin_game_over_survie(void);
int survie(int mode_difficulte);
int reinitialisation_survie(int mode_difficulte);
int dessin_pause_survie(void);
int set_start_survie(void);
int set_game_over_survie(void);
int input_high_score_survie(void);
int dessin_high_score_survie(void);
int set_high_score_survie(void);
int dessin_background_high_score_survie(void);
int write_high_score_survie(void);
int initialisation_position_main_survie(void);

#endif