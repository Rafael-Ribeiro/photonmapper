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
#include "Box.hpp"

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
	Material light =			Material(Color(255, 255, 255),		1.0, 1.0, 1.0, 1.492);

	Material glass =			Material(Color(255, 255, 255),		0.02, 0.02, 0.0, 1.492);
	Material mirror =			Material(Color(255, 255, 255),		0.02, 0.02, 0.0, infinity);

	Material redGlass =			Material(Color(255,   0,   0),		0.02, 0.02, 0.0, 1.492);
	Material greenGlass =		Material(Color(  0, 255,   0),		0.02, 0.02, 0.0, 1.492);
	Material blueGlass =		Material(Color(  0,   0, 255),		0.02, 0.02, 0.0, 1.492);
	Material blackGlass =		Material(Color(  0,   0, 0),		0.02, 0.02, 0.0, 1.492);

	Material glossyBlue = 		Material(Color(0, 0, 255),			0.4,  0.60, 0.0, infinity);

	Material grayConcrete = 	Material(Color(200, 200, 200),		1.0, 0.40, 0.0, infinity);
	Material blackConcrete = 	Material(Color(0, 0, 0),			1.0, 0.80, 0.0, infinity);

	Material redConcrete = 		Material(Color(255, 0, 0),			1.0, 0.60, 0.0, infinity);
	Material greenConcrete =	Material(Color(0, 255, 0),			1.0, 0.60, 0.0, infinity);
	Material blueConcrete = 	Material(Color(0, 0, 255),			1.0, 0.60, 0.0, infinity);

	/* Primitives */
	Quad q1 	= Quad(light, Point(10, 39, 10), Point(-10, 39, 10), Point(10, 39, -10));
	
	Sphere s0 	= Sphere(glass,		Point(  -5.0,	-25.0,	0),	15.0);
	Sphere s1 	= Sphere(mirror,	Point(	20.0,	-33.0,	0),	7.0); //Point( 10.0,	0.0,	0.0),	5.0)
	Box b1		= Box(glossyBlue, 	Point( -20, -39.99, 20), Vector(0, 40.0, 0), Vector(-15, 0.0, 10), Vector(10, 0, 15)); 
	
	Plane p1 = Plane(grayConcrete,		Point(0.0, -40.0, 0.0),	Vector(0.0, 1.0, 0.0)); 		/* floor */
	Plane p2 = Plane(grayConcrete,		Point(0.0, +40.0, 0.0),	Vector(0.0, -1.0, 0.0));	 	/* ceil */
	Plane p3 = Plane(redConcrete,		Point(-40.0, 0.0, 0.0),	Vector(1.0, 0.0, 0.0)); 		/* left */
	Plane p4 = Plane(greenConcrete,		Point(+40.0, 0.0, 0.0),	Vector(-1.0, 0.0, 0.0));	 	/* right */
	Plane p5 = Plane(grayConcrete,		Point(  0.0, 0.0, +40),	Vector(0.0, 0.0, -1.0)); 		/* front */
	Plane p6 = Plane(grayConcrete,		Point(  0.0, 0.0, -80),	Vector(0.0, 0.0, 1.0)); 		/* back */

	scene.lights.push_back(&q1);
	scene.primitives.push_back(&q1);

	scene.primitives.push_back(&b1);

	scene.primitives.push_back(&s0);
	scene.primitives.push_back(&s1);
	
	scene.primitives.push_back(&p1);
	scene.primitives.push_back(&p2);
	scene.primitives.push_back(&p3);
	scene.primitives.push_back(&p4);
	scene.primitives.push_back(&p5);
	scene.primitives.push_back(&p6);

	engine = Engine(scene);

	cameras.push_back(Camera(Point(0, 1, -60.02), Vector(0,0,1), Vector(0,1,0), M_PI/4, WIDTH, HEIGHT));

	for (camera = cameras.begin(); camera != cameras.end(); camera++)
	{
		pixels = engine.render(*camera);
		writePPM(pixels, camera->width, camera->height, std::cout);

		delete pixels;
	}

	return 0;
}
