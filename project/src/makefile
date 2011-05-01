FLAGS = -Wall

all: main.o Vector.o
	g++ Vector.o main.o -o raytracer ${FLAGS}

main.o: main.cpp
	g++ main.cpp -c -o main.o ${FLAGS}

Vector.o: Vector.cpp Vector.hpp
	g++ Vector.cpp -c -o Vector.o ${FLAGS}

clean:
	rm -f *.o ~* raytracer
