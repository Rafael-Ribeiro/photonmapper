#include <iostream>

#include "Vector.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "utils.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"

#define WIDTH 1024
#define HEIGHT 512

int main()
{
	Scene scene;

	Material glass = Material(Color(255, 255, 255), 0.02, 0.9, 0.0, 1.492);
	Material light = Material(Color(255, 255, 255), 1.0, 0.0, 1.0, 0.0);
	Material redConcrete = Material(Color(255, 0, 0), 0.7, 0.0, 0.0, 0.0);
	/* TODO: greenWall, blueWall */
	
	Sphere s1 = Sphere(glass, Point(0, 1.0, 0), 1.0);
	Sphere l1 = Sphere(light, Point(1.0, 3.0, 1.0), 0.1);
	
	Plane p1 = Plane(redConcrete, Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));
	
	/* TODO: add light to the primitive list too? */
	scene.primitives.push_back(&s1);
	scene.primitives.push_back(&p1);
	scene.lights.push_back(&l1);

	Engine engine = Engine(scene);

	Color* pixels = engine.render(Point(0,0,0),Vector(1,0,0), Vector(0,1,0), 60.0, WIDTH, HEIGHT);
	
	writePPM(pixels, WIDTH, HEIGHT, std::cout);

	return 0;
}
