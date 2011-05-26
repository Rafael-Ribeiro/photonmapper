FLAGS = -Wall -ansi -g

all: main.o Engine.o Scene.o Vector.o Color.o Plane.o Sphere.o Primitive.o Material.o utils.o Ray.o Photon.o
	g++ main.o Engine.o Scene.o Vector.o Color.o Plane.o Sphere.o Primitive.o Material.o utils.o Ray.o Photon.o -o renderer ${FLAGS}

main.o: main.cpp
	g++ main.cpp -c -o main.o ${FLAGS}

Vector.o: Vector.cpp Vector.hpp
	g++ Vector.cpp -c -o Vector.o ${FLAGS}

Color.o: Color.cpp Color.hpp
	g++ Color.cpp -c -o Color.o ${FLAGS}

Primitive.o: Primitive.cpp Primitive.hpp Material.hpp
	g++ Primitive.cpp -c -o Primitive.o ${FLAGS}

Material.o: Material.cpp Material.hpp
	g++ Material.cpp -c -o Material.o ${FLAGS}

Plane.o: Plane.cpp Plane.hpp
	g++ Plane.cpp -c -o Plane.o ${FLAGS}

Sphere.o: Sphere.cpp Sphere.hpp
	g++ Sphere.cpp -c -o Sphere.o ${FLAGS}

Scene.o: Scene.cpp Scene.hpp
	g++ Scene.cpp -c -o Scene.o ${FLAGS}

Engine.o: Engine.cpp Engine.hpp
	g++ Engine.cpp -c -o Engine.o ${FLAGS}

utils.o: utils.cpp utils.hpp
	g++ utils.cpp -c -o utils.o ${FLAGS}

Ray.o: Ray.cpp Ray.hpp
	g++ Ray.cpp -c -o Ray.o ${FLAGS}

Photon.o: Photon.cpp Photon.hpp
	g++ Photon.cpp -c -o Photon.o ${FLAGS}

clean:
	rm -f *.o ~* renderer
