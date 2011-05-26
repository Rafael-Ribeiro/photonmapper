#include "Engine.hpp"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

Engine::Engine(Scene& scene)
	: scene(scene)
{
	this->nPhotons = MAX_PHOTONS;
	this->nPhotonBounce = MAX_PHOTON_BOUNCE;
}

Color* Engine::render(Point origin, Vector direction, Vector top, double fovy, int width, int height)
{
	Color* pixels;
	int i, j;
	vector<Photon>::iterator it;
	Vector left;
	Ray ray;
	double dx, dy, aspect;
	double halfY, halfX;

	pixels = new Color[width*height];

	/* TODO photon mapping */
	this->scene.buildPhotonMap(nPhotons, nPhotonBounce);

	aspect = (1.0 * width) / height;
	top = top*tan(fovy);
	left = direction.cross(top)*tan(fovy)*aspect;

	ray.origin = origin;
	halfY = height / 2.0;
	halfX = width / 2.0;

	for (i = 0; i < height; i++)
	{
		dy = (-i+0.5+halfY)/halfY;

		for (j = 0; j < width; j++)
		{
			dx = (j+0.5-halfX)/halfX;

			ray.direction = (direction + top*dy + left*dx).normalize();
			pixels[i*width + j] = ray.getColor(scene, 1, N_AIR);
		}
	}

/*
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
			pixels[i*width + j] = Color(0,0,0);
	}

	for (it = this->scene.photonMap.begin(); it != this->scene.photonMap.end(); it++)
	{
		int x = (it->ray.origin.x + 60)*512.0/80;
		int y = (it->ray.origin.z + 120)*256.0/80;

		if (x>= 0 && y >= 0 && x < width && y < height)
			pixels[y*width+x] = it->color;
	}
*/	
	/* TODO anti-aliasing */

	return pixels;
}
