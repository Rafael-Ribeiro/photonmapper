#include "Engine.hpp"

#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

Engine::Engine(Scene& scene)
	: scene(scene)
{
	this->nPhotons = Engine::MAX_PHOTONS;
	this->nPhotonBounce = Engine::MAX_PHOTON_BOUNCE;
}

Color* Engine::render(Camera camera)
{
	Color* pixels;
	int i, j;
	//vector<Photon>::iterator it;

	pixels = new Color[camera.width * camera.height];

	this->scene.buildPhotonMap(nPhotons, nPhotonBounce);

	for (i = 0; i < camera.height; i++)
		for (j = 0; j < camera.width; j++)
			pixels[i * camera.width + j] = camera.rayTroughPixel(j, i).getColor(scene, Engine::MAX_RAY_BOUNCE, N_AIR);

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
