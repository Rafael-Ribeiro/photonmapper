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
	unsigned int i, j, n, sum;
	vector<Photon> photons;

	pixels = new Color[width*height];

	/* TODO photon mapping */
	sum = 0;
	for (i = 0; i < this->scene.lights.size(); i++)
		sum += this->scene.lights[i]->area() * this->scene.lights[i]->mat.emittance;
	
	for (i = 0; i < this->scene.lights.size(); i++)
	{
		n = this->scene.lights[i]->area() * this->scene.lights[i]->mat.emittance*this->nPhotons/sum;
		
		for (j = 0; j < n; j++)
			photons.push_back(this->scene.lights[i]->randomPhoton().bounce(this->scene, this->nPhotonBounce));
	}

	/* TODO build KD?-tree */

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
