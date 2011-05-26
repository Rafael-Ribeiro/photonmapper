#include "Scene.hpp"

Scene::Scene()
{
}

bool Scene::intersect(Ray& p, Intersection& intersect) const
{
	/* TODO: intersects here */
	return false;
}

void Scene::buildPhotonMap(unsigned int nPhotons, unsigned int nPhotonBounce)
{
	/* TODO build KD-tree */

	unsigned int i, j, n;
	double sum;
	Photon photon;

	this->photonMap.clear();

	sum = 0;
	for (i = 0; i < this->lights.size(); i++)
		sum += this->lights[i]->area() * this->lights[i]->mat.emittance;

	for (i = 0; i < this->lights.size(); i++)
	{
		n = this->lights[i]->area() * this->lights[i]->mat.emittance*nPhotons/sum;
	
		for (j = 0; j < n; j++)
			if (this->lights[i]->randomPhoton().bounce(*this, nPhotonBounce, photon, N_AIR))
				this->photonMap.push_back(photon);

	}
}
