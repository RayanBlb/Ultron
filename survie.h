#ifndef SURVIE_H_
#define SURVIE_H_

typedef struct position_struct_survie{
	int x;
	int y;
}position_survie;

int init_survie(void);
void input_survie(void);
void update_survie(void);
void set_survie(void);
void dessin_fond_survie(void);
void get_screensize_survie(void);
void tab_deplacement_survie(int x, int y);
void dessin_background_score_survie(void);
void dessin_score_survie(void);
void dessin_game_over_survie(void);
void survie(int mode_difficulte);
void reinitialisation_survie(int mode_difficulte);
void dessin_pause_survie(void);
void set_game_over_survie(void);
void input_high_score_survie(void);
void dessin_high_score_survie(void);
void set_high_score_survie(void);
void dessin_background_high_score_survie(void);
void write_high_score_survie(void);
void initialisation_position_main_survie(int mode_difficulte);

#endif