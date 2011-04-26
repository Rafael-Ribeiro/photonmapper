all: main.o materials.o objects.o
	gcc main.o materials.o objects.o -o main -ansi -Wall -lglut -lGLU -g

main.o: main.c
	gcc main.c -c -o main.o -ansi -Wall -g

materials.o: materials.c
	gcc materials.c -c -o materials.o -ansi -Wall -g

objects.o: objects.c
	gcc objects.c -c -o objects.o -ansi -Wall -g 

clean:
	rm *.o main
