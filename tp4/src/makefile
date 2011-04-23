all: main.o materials.o
	gcc main.o materials.o -o main -ansi -Wall -lglut -lGLU 

main.o: main.c
	gcc main.c -c -o main.o -ansi -Wall

materials.o: materials.c
	gcc materials.c -c -o materials.o -ansi -Wall

