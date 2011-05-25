#include "Engine.hpp"
#include <vector>
#include <iostream>

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
	vector<Photon>::iterator it;

	pixels = new Color[width*height];

	/* TODO photon mapping */
	scene.buildPhotonMap(nPhotons, nPhotonBounce);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			/* cast ray */
			pixels[i*width+j] = Color(0,0,0);
		}
	}

	/* TEST */
	for (it = this->scene.photonMap.begin(); it != this->scene.photonMap.end(); it++)
	{
		unsigned int x = (it->ray.origin.x-1)*1000+512;
		unsigned int y = (it->ray.origin.z-1)*1000+256;

		//cerr << it->ray.origin.x << " " << it->ray.origin.z << endl;

		pixels[y*width+x] = Color(255, 255, 255);
	}

	/* TODO anti-aliasing */

	return pixels;
}
