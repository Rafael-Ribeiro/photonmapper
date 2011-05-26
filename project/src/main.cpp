#include <iostream>
#include <cmath>

#include "Vector.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "utils.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"

#define WIDTH 800
#define HEIGHT 600

int main()
{
	Scene scene;

	Material glass = Material(Color(255, 255, 255), 0.02, 0.9, 0.0, 1.492);
	Material light = Material(Color(255, 255, 255), 1.0, 0.0, 1.0, 0.0);
	Material redConcrete = Material(Color(255, 0, 0), 0.7, 0.0, 0.0, 0.0);
	Material grayConcrete = Material(Color(0xa9, 0xa9, 0xa9), 0.7, 0.0, 0.0, 0.0);
	Material greenConcrete = Material(Color(0, 255, 0), 0.7, 0.0, 0.0, 0.0);

	/* TODO: greenWall, blueWall */
	
	Sphere s1 = Sphere(redConcrete, Point(0.0, 0.0, 10.0), 1);
	Sphere l1 = Sphere(light, Point(1.0, 2.0, 5.0), 0.5);
	
	Plane p1 = Plane(grayConcrete, Point(0.0, -40.0, 0.0), Vector(0.0, 1.0, 0.0)); 		/* floor */
	Plane p2 = Plane(grayConcrete, Point(0.0, +40.0, 0.0), Vector(0.0, -1.0, 0.0)); 	/* ceil */
	Plane p3 = Plane(redConcrete, Point(-40.0, 0.0, 0.0), Vector(-1.0, 0.0, 0.0)); 		/* left */
	Plane p4 = Plane(greenConcrete, Point(+40.0, 0.0, 0.0), Vector(-1.0, 0.0, 0.0)); 	/* right */
	Plane p5 = Plane(grayConcrete, Point(0.0, 0.0, +400), Vector(0.0, 0.0, -1.0)); 		/* front */
//	Plane p6 = Plane(grayConcrete, Point(0.0, 0.0, -40.0), Vector(0.0, 0.0, 1.0)); 		/* back */

	scene.lights.push_back(&l1);

	scene.primitives.push_back(&l1);
	scene.primitives.push_back(&s1);

	scene.primitives.push_back(&p1);
	scene.primitives.push_back(&p2);
	scene.primitives.push_back(&p3);
	scene.primitives.push_back(&p4);
	scene.primitives.push_back(&p5);
//	scene.primitives.push_back(&p6);

	Engine engine = Engine(scene);
	
	/* TODO: aperture */
	Color* pixels = engine.render(Point(0,0,0), Vector(0,0,1), Vector(0,1,0), 1, WIDTH, HEIGHT);
	
	writePPM(pixels, WIDTH, HEIGHT, std::cout);

	return 0;
}
