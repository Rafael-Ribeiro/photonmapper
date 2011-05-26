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
	double dx, dy, fovx, aspect;
	double halfY, halfX;

	pixels = new Color[width*height];

	/* TODO photon mapping */
	this->scene.buildPhotonMap(nPhotons, nPhotonBounce);

	aspect = (1.0 * width) / height;
	fovx = fovy * aspect;
	left = direction.cross(top)*sin(fovx);
	top = top*sin(fovy);

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
			pixels[i*width + j] = ray.getColor(scene, 1);
		}
	}

	/* TEST */
	/*for (it = this->scene.photonMap.begin(); it != this->scene.photonMap.end(); it++)
	{
		unsigned int x = (it->ray.origin.x)*50+512;
		unsigned int y = (it->ray.origin.z)*50+256;

		//cerr << it->ray.origin.x << " " << it->ray.origin.z << endl;

		if (x < 1024 && y < 512)
			pixels[y*width+x] = Color(255, 255, 255);
	}
	*/
	/* TODO anti-aliasing */

	return pixels;
}
