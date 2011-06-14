#include <iostream>
#include <cmath>
#include <limits>

#include "Vector.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "Camera.hpp"
#include "utils.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Quad.hpp"

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
	double infinity = numeric_limits<double>::infinity();

	/* Materials */
	/* Material(Color color, double roughness, double absorvance, double emittance, double n) */
	Material light =			Material(Color(255, 255, 255),		0.3, 1.0, 1.0, 1.492);

	Material glass =			Material(Color(255, 255, 255),		0.02, 0.02, 0.0, 1.492);
	Material mirror =			Material(Color(255, 255, 255),		0.02, 0.02, 0.0, infinity);

	Material grayConcrete = 	Material(Color(0xa6, 0xa6, 0xa6),	0.3, 0.95, 0.0, infinity);
	Material blackConcrete = 	Material(Color(0, 0, 0),			0.3, 0.95, 0.0, infinity);

	Material redConcrete = 		Material(Color(255, 0xa6, 0xa6),	0.3, 0.95, 0.0, infinity);
	Material greenConcrete =	Material(Color(0xa6, 255, 0xa6),	0.3, 0.95, 0.0, infinity);
	Material blueConcrete = 	Material(Color(0xa6, 0xa6, 255),	0.3, 0.95, 0.0, infinity);

	/* Primitives */
	Sphere s1 	= Sphere(glass,		Point(0.0, -0.0, 0.0),	20.0);
	Sphere s2 	= Sphere(mirror,	Point(+30.0, -30.0, 30.0),	10.0);
	Quad q1 	= Quad(light, 		Point(10, 39, 10), Point(-10, 39, 10), Point(10, 39, -10));

	Plane p1 = Plane(grayConcrete,	Point(0.0, -40.0, 0.0),	Vector(0.0, 1.0, 0.0)); 		/* floor */
	Plane p2 = Plane(grayConcrete,	Point(0.0, +40.0, 0.0),	Vector(0.0, -1.0, 0.0));	 	/* ceil */
	Plane p3 = Plane(redConcrete,	Point(-40.0, 0.0, 0.0),	Vector(1.0, 0.0, 0.0)); 		/* left */
	Plane p4 = Plane(greenConcrete,	Point(+40.0, 0.0, 0.0),	Vector(-1.0, 0.0, 0.0));	 	/* right */
	Plane p5 = Plane(blueConcrete,	Point(0.0, 0.0, +40),	Vector(0.0, 0.0, -1.0)); 		/* front */
	Plane p6 = Plane(blackConcrete,	Point(0.0, 0.0, -41),	Vector(0.0, 0.0, 1.0)); 		/* back */

	scene.lights.push_back(&q1);

	scene.primitives.push_back(&q1);	
	scene.primitives.push_back(&s1);
	scene.primitives.push_back(&s2);

	scene.primitives.push_back(&p1);
	scene.primitives.push_back(&p2);
	scene.primitives.push_back(&p3);
	scene.primitives.push_back(&p4);
	scene.primitives.push_back(&p5);
	scene.primitives.push_back(&p6);

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
