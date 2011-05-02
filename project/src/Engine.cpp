#include "Engine.hpp"

Engine::Engine(Scene& scene)
	: scene(scene)
{
}

Color* Engine::render(Point origin, Vector direction, Vector top, double apperture, int width, int height)
{
	Color* pixels;
	int i, j;

	pixels = new Color[width*height];

	/* TODO build KD?-tree */

	/* TODO photon mapping */

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			/* cast ray */
			pixels[i*width+j] = Color(i%255, j%255, (i+j)%255);
		}
	}

	/* TODO anti-aliasing */

	return pixels;
}
