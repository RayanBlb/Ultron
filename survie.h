#ifndef SURVIE_H_
#define SURVIE_H_

int init_survie(void);
int input_survie(void);
int update_survie(void);
int set_survie(void);
int delay_game_survie(void);
int dessin_main_survie(void);
int dessin_fond_survie(void);
int get_screensize_survie(void);
int tab_deplacement_survie(int x, int y);
int fermeture_sdl_survie(void);
int dessin_background_score_survie(void);
int dessin_score_survie(void);
int dessin_game_over_survie(void);
int play_musique_survie(void);
int play_explosion_survie(void);
int switch_screen_survie(void);
int survie(void);
int reinitialisation_survie(void);
int dessin_pause_survie(void);
int set_start_survie(void);
int set_game_over_survie(void);
int allocation_tableau_survie(void);
int free_tableau_survie(void);

#endif