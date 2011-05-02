#include <iostream>

#include "Vector.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "PPM.hpp"

#define WIDTH 1020
#define HEIGHT 765

int main()
{
	Scene scene;
	/* TODO: build scene */

	Engine engine = Engine(scene);
	/* TODO: build engine */

	Color* pixels = engine.render(Point(0,0,0),Vector(1,0,0), Vector(0,1,0), 60.0, WIDTH, HEIGHT);
	/* TODO: render */
	
	/* TODO: draw image */
	writePPM(pixels, WIDTH, HEIGHT, std::cout);

	return 0;
}
