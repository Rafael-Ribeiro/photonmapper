#include "Engine.hpp"
#include <vector>

using namespace std;

Engine::Engine(Scene& scene)
	: scene(scene)
{
	this->nPhotons = MAX_PHOTONS;
	this->nPhotonBounce = MAX_PHOTON_BOUNCE;
}

Color* Engine::render(Point origin, Vector direction, Vector top, double apperture, unsigned int width, unsigned int height)
{
	Color* pixels;
	unsigned int i, j;

	pixels = new Color[width*height];

	/* TODO photon mapping */
	scene.buildPhotonMap(nPhotons, nPhotonBounce);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			/* cast ray */
			pixels[i*width+j] = Color(i%256, j%256, (i+j)%256);
		}
	}

	/* TODO anti-aliasing */

	return pixels;
}
