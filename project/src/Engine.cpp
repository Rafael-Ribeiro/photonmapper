#include "Engine.hpp"

#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

Engine::Engine()
{
}

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
	vector<Photon>::iterator it;

	pixels = new Color[camera.width * camera.height];

	this->scene.buildPhotonMap(nPhotons, nPhotonBounce);
	
	/*for (i = 0; i < camera.height; i++)
		for (j = 0; j < camera.width; j++)
			pixels[i * camera.width + j] = camera.rayTroughPixel(j, i).getColor(scene, Engine::MAX_RAY_BOUNCE, N_AIR);
	
	*/
	for (i = 0; i < camera.height; i++)
	{
		for (j = 0; j < camera.width; j++)
			pixels[i*camera.width + j] = Color(0,0,0);
	}

	for (it = this->scene.photonMap.begin(); it != this->scene.photonMap.end(); it++)
	{
		cerr 	<< it->ray.origin.x << " " << it->ray.origin.y << " " << it->ray.origin.z << " | "
				<< (int)it->color.r << " " << (int)it->color.g << " " << (int)it->color.b << " | " <<endl;

		int x = (it->ray.origin.x + 60)*512.0/200;
		int y = (it->ray.origin.z + 120)*512.0/200;

		if (x>= 0 && y >= 0 && x < camera.width && y < camera.height)
			pixels[y*camera.width+x] = it->color;
	}
	

	/* TODO anti-aliasing */

	return pixels;
}
