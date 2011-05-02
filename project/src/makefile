FLAGS = -Wall -ansi

all: main.o Engine.o Scene.o Vector.o Plane.o Sphere.o Primitive.o
	g++ main.o Engine.o Scene.o Vector.o Plane.o Sphere.o Primitive.o -o renderer ${FLAGS}

main.o: main.cpp
	g++ main.cpp -c -o main.o ${FLAGS}

Vector.o: Vector.cpp Vector.hpp
	g++ Vector.cpp -c -o Vector.o ${FLAGS}

Primitive.o: Primitive.cpp Primitive.hpp Material.hpp
	g++ Primitive.cpp -c -o Primitive.o ${FLAGS}

Plane.o: Plane.cpp Plane.hpp
	g++ Plane.cpp -c -o Plane.o ${FLAGS}

Sphere.o: Sphere.cpp Sphere.hpp
	g++ Sphere.cpp -c -o Sphere.o ${FLAGS}

Scene.o: Scene.cpp Scene.hpp
	g++ Scene.cpp -c -o Scene.o ${FLAGS}

Engine.o: Engine.cpp Engine.hpp
	g++ Engine.cpp -c -o Engine.o ${FLAGS}

clean:
	rm -f *.o ~* renderer
