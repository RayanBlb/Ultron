#ifndef IA_H_
#define IA_H_

typedef struct position_struct_ia{
	int x;
	int y;
}position_ia;

void input_ia(void);
void update_ia(void);
void set_ia(void);
void dessin_main_ia(void);
void dessin_deuxieme_ia(void);
void dessin_fond_ia(void);
void get_screensize_ia(void);
void dessin_background_score_ia(void);
void dessin_score_ia(void);
void dessin_game_over_ia(void);
void ia(int mode_difficulte,int mode_ia);
void reinitialisation_ia(int mode);
void dessin_pause_ia(void);
void set_start_ia(void);
void set_game_over_ia(void);
void input_high_score_ia(void);
void dessin_high_score_ia(void);
void set_high_score_ia(void);
void dessin_background_high_score_ia(void);
void write_high_score_ia(void);
void initialisation_position_main_ia(int mode_ia, int mode_difficulte);
void brain_ia(void);
void choix_depart_ia(void);
void changement_aleatoire_ia(void);
void brain_miroir_ia(void);
void brain_classique_ia(void);
void choix_mode_ia(int mode_ia);
int chance_ia(int choix1,int choix2);
int tab_deplacement_ia(int x, int y,int joueur);
int init_ia(void);

#endif