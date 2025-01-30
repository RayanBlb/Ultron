# Ultron

Ultron est un jeu d'arcade minimaliste où le joueur incarne un carré blanc dont l'objectif est de survivre le plus longtemps possible sur la carte.

## Fonctionnalités
- **Trois modes de jeu** :
  - **Survie solo** : tenez le plus longtemps possible.
  - **1v1 local** : un joueur contrôle le carré blanc (**ZQSD**) et l'autre le carré rouge (**flèches directionnelles**).
  - **VS IA** : affrontez l'une des trois IA disponibles.
- **Trois types d'IA** :
  - **IA classique** : tente de survivre en esquivant l'IA, qui ne suit pas de modèle particulier.
  - **IA "mur vertical"** : l'IA tente de vous bloquer en construisant un mur vertical.
  - **IA "miroir"** : l'IA imite chacun des mouvements du joueur.
- **Trois niveaux de difficulté** :
  - **Facile** : carte de grande taille.
  - **Normale** : carte de taille intermédiaire.
  - **Difficile** : carte très petite qui rend la survie complexe.
- **Système de high scores** :
  - Un classement est enregistré pour chaque mode de jeu.
  - Possibilité d'entrer son nom à la fin de chaque partie.
  - Possibilité de réinitialiser les high scores en appuyant sur la touche **S**.

---

## Installation et Compilation

### Prérequis
Assurez-vous d'avoir installé les bibliothèques SDL2 suivantes :
- **SDL2**
- **SDL2_ttf**
- **SDL2_mixer**
- **SDL2_image**

### Compilation
Un `Makefile` est fourni pour simplifier la compilation. 

Utilisez simplement la commande :

```sh
make
```

---

## Musique
- **Titre** : Synthwave Vibe
- **Auteur** : Meydän
- **Source** : [Meydan Bandcamp](https://meydan.bandcamp.com)
- **Licence** : [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/deed.fr)
- **Téléchargement (5MB)** : [auboutdufil.com](https://auboutdufil.com/?id=504)

---

## Comment jouer ?
- **Joueur solo** : utilisez les touches directionnelles **ZQSD** pour vous déplacer.
- **Mode 1v1** :
  - Joueur blanc : **ZQSD**
  - Joueur rouge : **Flèches directionnelles**
- **Suppression des high scores** : appuyez sur la touche **S**
