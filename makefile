prog: main.o menu.o perso.o background.o ennemie.o map.o enigme.o puzzle.o
	gcc $^ -o prog -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lm -g

main.o: main.c menu.h
	gcc -c main.c -g

menu.o: menu.c menu.h
	gcc -c menu.c -g

perso.o: perso.c perso.h
	gcc -c perso.c -g

background.o: background.c background.h
	gcc -c background.c -g

ennemie.o: ennemie.c ennemie.h
	gcc -c ennemie.c -g

map.o: map.c map.h
	gcc -c map.c -g

enigme.o: enigme.c enigme.h
	gcc -c enigme.c -g

puzzle.o: puzzle.c puzzle.h
	gcc -c puzzle.c -g

clean:
	rm -f *.o prog
