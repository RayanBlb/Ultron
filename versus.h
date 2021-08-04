#ifndef VERSUS_H_
#define VERSUS_H_

typedef struct position_struct_versus{
	int x;
	int y;
}position_versus;

void input_versus(void);
void update_versus(void);
void set_versus(void);
void dessin_fond_versus(void);
void get_screensize_versus(void);
void dessin_background_score_versus(void);
void dessin_score_versus(void);
void dessin_game_over_versus(void);
void versus(int mode);
void reinitialisation_versus(int mode);
void dessin_pause_versus(void);
void set_game_over_versus(void);
void input_high_score_versus(void);
void dessin_high_score_versus(void);
void set_high_score_versus(void);
void dessin_background_high_score_versus(void);
void write_high_score_versus(void);
void initialisation_position_main_versus(int mode_difficulte);
int init_versus(void);
int tab_deplacement_versus(int x, int y,int joueur);

#endif