output: main.o difficulte.o high_score.o ia.o menu.o mode_ia.o outils.o survie.o versus.o
	gcc main.o difficulte.o high_score.o ia.o menu.o mode_ia.o outils.o survie.o versus.o -o prog -I include -L lib -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image

main.o: main.c
	gcc -c main.c

difficulte.o: difficulte.c difficulte.h
	gcc -c difficulte.c

high_score.o: high_score.c high_score.h
	gcc -c high_score.c

ia.o: ia.c ia.h
	gcc -c ia.c

menu.o: menu.c menu.h
	gcc -c menu.c

mode_ia.o: mode_ia.c mode_ia.h
	gcc -c mode_ia.c

outils.o: outils.c outils.h
	gcc -c outils.c

survie.o: survie.c survie.h
	gcc -c survie.c

versus.o: versus.c versus.h
	gcc -c versus.c

clean: 
	rm *.o prog