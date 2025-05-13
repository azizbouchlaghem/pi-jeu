prog: main.o enigme.o puzzle.o
	gcc main.o enigme.o puzzle.o -o prog -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lm

main.o: main.c enigme.h puzzle.h
	gcc -c main.c

enigme.o: enigme.c enigme.h
	gcc -c enigme.c

puzzle.o: puzzle.c puzzle.h
	gcc -c puzzle.c

clean:
	rm -f *.o prog
