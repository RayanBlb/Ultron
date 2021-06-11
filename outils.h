#ifndef OUTILS_H_
#define OUTILS_H_

typedef struct liste{
	int score;
	char *nom;
	struct liste *pNext;
}liste;

int free_liste_high_score_outils(liste **pFirst);
int read_file_high_score_outils(char *chemin,char *nom_high_score_survie,int score_survie,liste *pFirst);
int write_liste_high_score_outils(liste *pFirst,char *chemin);
int tri_high_score_outils(liste **pFirst,char *nom,int score);
int write_file_outils(char *nom, int score,char *chemin);
int sup_liste_high_score_outils(char *chemin);

#endif