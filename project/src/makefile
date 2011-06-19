FLAGS = -O3 -fopenmp -Wall -ansi -g
JSONBOX_FILES = jsonbox/Array.o jsonbox/Convert.o jsonbox/Object.o jsonbox/Value.o

all: clean main.o Engine.o Scene.o Vector.o Camera.o Color.o Plane.o Sphere.o Primitive.o Material.o utils.o Ray.o Photon.o Quad.o Box.o JsonBox
	g++ main.o Engine.o Scene.o Vector.o Camera.o Color.o Plane.o Sphere.o Primitive.o Material.o utils.o Ray.o Photon.o Quad.o Box.o ${JSONBOX_FILES} -o renderer ${FLAGS}

main.o: main.cpp
	g++ main.cpp -c -o main.o ${FLAGS}

Vector.o: Vector.cpp Vector.hpp
	g++ Vector.cpp -c -o Vector.o ${FLAGS}

Camera.o: Camera.cpp Camera.hpp
	g++ Camera.cpp -c -o Camera.o ${FLAGS}

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

Quad.o: Quad.cpp Quad.hpp
	g++ Quad.cpp -c -o Quad.o ${FLAGS}

Box.o: Box.cpp Box.hpp
	g++ Box.cpp -c -o Box.o ${FLAGS}

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


JsonBox: JsonBox_Array.o JsonBox_Convert.o JsonBox_Object.o JsonBox_Value.o

JsonBox_Array.o:
	g++ jsonbox/Array.cpp -c -o jsonbox/Array.o ${FLAGS}

JsonBox_Convert.o:
	g++ jsonbox/Convert.cpp -c -o jsonbox/Convert.o ${FLAGS}

JsonBox_Object.o:
	g++ jsonbox/Object.cpp -c -o jsonbox/Object.o ${FLAGS}

JsonBox_Value.o:
	g++ jsonbox/Value.cpp -c -o jsonbox/Value.o ${FLAGS}


validation: box_test

box_test: clean validations/box_test.o Engine.o Scene.o Vector.o Camera.o Color.o Plane.o Sphere.o Primitive.o Material.o utils.o Ray.o Photon.o Quad.o Box.o
	g++ validations/box_test.o Engine.o Scene.o Vector.o Camera.o Color.o Plane.o Sphere.o Primitive.o Material.o utils.o Ray.o Photon.o Quad.o Box.o -o validations/box_test ${FLAGS}

box_test.o: validations/box_test.cpp
	g++ validations/box_test.cpp -c -o validations/box_test.o ${FLAGS}

clean:
	rm -f *.o ~* renderer
