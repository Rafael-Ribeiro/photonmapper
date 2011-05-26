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
	Vector right;
	Ray ray;
	double dx, dy;

	pixels = new Color[width*height];

	/* TODO photon mapping */
	scene.buildPhotonMap(nPhotons, nPhotonBounce);

	right = direction.cross(top); /* left maybe? */
 	cerr << right.x << " " << right.y << " " << right.z << endl;

	ray.origin = origin;	
	for (i = 0; i < height; i++)
	{
		dy = -(i - height/2.0) - 0.5;

		for (j = 0; j < width; j++)
		{
			/* cast ray */
			dx = (i - width/2.0) - 0.5;

			ray.direction = direction + right*(dx*apperture) + top*(dy*apperture);
			pixels[i*width+j] = ray.getColor(scene, 1);
		}
	}

	/* TEST */
	for (it = this->scene.photonMap.begin(); it != this->scene.photonMap.end(); it++)
	{
		unsigned int x = (it->ray.origin.x)*50+512;
		unsigned int y = (it->ray.origin.z)*50+256;

		//cerr << it->ray.origin.x << " " << it->ray.origin.z << endl;

		if (x < 1024 && y < 512)
			pixels[y*width+x] = Color(255, 255, 255);
	}

	/* TODO anti-aliasing */

	return pixels;
}
