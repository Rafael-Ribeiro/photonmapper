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

Color* Engine::render(Camera camera, int width, int height)
{
	Color* pixels;
	int i, j;
	vector<Photon>::iterator it;
	Vector top, right;
	Ray ray;
	double dx, dy, aspect;
	double halfY, halfX;

	pixels = new Color[width*height];

	/* TODO photon mapping */
	this->scene.buildPhotonMap(nPhotons, nPhotonBounce);

	aspect = (1.0 * width) / height;
	top = camera.top * tan(camera.fovy);
	right = camera.direction.cross(top) * tan(camera.fovy) * (-aspect);

	ray.origin = camera.origin;
	halfY = height / 2.0;
	halfX = width / 2.0;

	for (i = 0; i < height; i++)
	{
		dy = (-i+0.5+halfY)/halfY;

		for (j = 0; j < width; j++)
		{
			dx = (j+0.5-halfX)/halfX;

			ray.direction = (camera.direction + top*dy + right*dx).normalize();
			pixels[i*width + j] = ray.getColor(scene, MAX_RAY_BOUNCE, N_AIR);
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
