#include <float.h>

#include "Scene.hpp"
#include "Primitive.hpp"

using namespace std;

Scene::Scene()
{
}

/* TODO: make this use a kd-tree*/
bool Scene::intersect(Ray& ray, Intersection& intersect) const 
{
	vector<Primitive*>::const_iterator it, end;
	double sqrd_dist, temp_dist;
	Point inter_point;
	Vector delta;

	sqrd_dist = DBL_MAX;
	for (it = this->primitives.begin(), end = this->primitives.end() ; it != end; it++)
	{
		if ((*it)->intersect(ray, inter_point))
		{
			delta = inter_point - ray.origin;
			temp_dist = delta.sqrd_norm();

			if (temp_dist < sqrd_dist)
			{
				intersect.point = inter_point;
				intersect.prim = *it;
				intersect.direction = ray.direction;

				temp_dist = sqrd_dist;
			}
		}
	}

	return sqrd_dist < DBL_MAX;
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
			if (this->lights[i]->randomPhoton().bounce(*this, nPhotonBounce, photon))
				this->photonMap.push_back(photon);

	}
}
