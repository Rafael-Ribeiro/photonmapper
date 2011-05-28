#include <iostream>
#include <cmath>

#include "Vector.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include "utils.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"

#define WIDTH 800
#define HEIGHT 600

/*
 * Scene assumes:
 *
 *			^ y
 *			|
 *			|    ^ z
 *			|   /
 *			|  /
 *			| /
 *			|/
 * 			+------------> x
 */

int main()
{
	Engine engine;
	Scene scene;

	vector<Camera> cameras;
	vector<Camera>::iterator camera;

	Color* pixels;

	/* Materials */
	/* Material(Color color, double roughness, double albedo, double refractance, double emittance, double n) */
	Material light =			Material(Color(255, 255, 255),		0.02, 1.0, 0, 1.0, 1.492);

	Material glass =			Material(Color(255, 255, 255),		0.02, 0.02, 0.9, 0.0, 1.492);
	Material mirror =			Material(Color(255, 255, 255),		0.02, 0.02, 0.0, 0.0, 20);

	Material redConcrete = 		Material(Color(255, 0, 0),			0.7, 0.7, 0.0, 0.0, 0.0);
	Material grayConcrete = 	Material(Color(0xa9, 0xa9, 0xa9),	0.7, 0.7, 0.0, 0.0, 0.0);
	Material greenConcrete =	Material(Color(0, 255, 0),			0.7, 0.7, 0.0, 0.0, 0.0);

	/* Primitives */
	Sphere s1 = Sphere(glass,		Point(-20.0, -20.0, 20.0),	20.0);
	Sphere l1 = Sphere(light,		Point(0, +30, 0),			10.0);

	Plane p1 = Plane(grayConcrete,	Point(0.0, -40.0, 0.0),	Vector(0.0, 1.0, 0.0)); 		/* floor */
	Plane p2 = Plane(grayConcrete,	Point(0.0, +40.0, 0.0),	Vector(0.0, -1.0, 0.0));	 	/* ceil */
	Plane p3 = Plane(redConcrete,	Point(-40.0, 0.0, 0.0),	Vector(1.0, 0.0, 0.0)); 		/* left */
	Plane p4 = Plane(greenConcrete,	Point(+40.0, 0.0, 0.0),	Vector(-1.0, 0.0, 0.0));	 	/* right */
	Plane p5 = Plane(grayConcrete,	Point(0.0, 0.0, +40),	Vector(0.0, 0.0, -1.0)); 		/* front */

	scene.lights.push_back(&l1);

	scene.primitives.push_back(&l1);
	scene.primitives.push_back(&s1);

	scene.primitives.push_back(&p1);
	scene.primitives.push_back(&p2);
	scene.primitives.push_back(&p3);
	scene.primitives.push_back(&p4);
	scene.primitives.push_back(&p5);

	engine = Engine(scene);

	cameras.push_back(Camera(Point(0, 0, -40), Vector(0,0,1), Vector(0,1,0), M_PI/4, WIDTH, HEIGHT));

	for (camera = cameras.begin(); camera != cameras.end(); camera++)
	{
		pixels = engine.render(*camera);
		writePPM(pixels, camera->width, camera->height, std::cout);

		delete pixels;
	}

	return 0;
}
