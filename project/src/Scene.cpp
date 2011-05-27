#include <float.h>
#include <iostream>
#include <algorithm>

#include "Scene.hpp"
#include "Primitive.hpp"

using namespace std;

Scene::Scene()
{
}

struct PointDistanceComparator
{
	Point p;

	PointDistanceComparator(Point p)
		: p(p)
	{
	}

	bool operator() (const Photon *p1, const Photon *p2) const
	{
		return (p1->ray.origin - this->p).sqrd_norm() < (p2->ray.origin - this->p).sqrd_norm();
	}
};

/* TODO: make this use a kd-tree*/
bool Scene::intersect(const Ray& ray, Intersection& intersect) const 
{
	vector<Primitive*>::const_iterator it, end;
	double sqrd_dist, temp_dist;
	Point inter_point;
	Vector delta;

	sqrd_dist = DBL_MAX;
	for (it = this->primitives.begin(), end = this->primitives.end(); it != end; it++)
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

				cerr << "DIR: " << intersect.direction.norm() << endl;
				sqrd_dist = temp_dist;
			}
		}
	}

	return sqrd_dist < DBL_MAX;
}

void Scene::buildPhotonMap(int nPhotons, int nPhotonBounce)
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
			this->lights[i]->randomPhoton().bounce(*this, nPhotonBounce, photon, N_AIR);	
	}

}

/*
 * TODO: getKNearestPhotons using KD-Tree
 * 
 * getKNearestPhotons is simply iterating the scene's photonMap
 * (which is a vector at the moment) and sorting Photon* by their distance to p,
 * to return the first k Photon* of the sort
 */
vector<const Photon*> Scene::getKNearestPhotons(Point p, int k) const
{
	int i;

	vector<const Photon*> knn;
	vector<const Photon*> sortedPhotons;

	vector<Photon>::const_iterator it, end;

	sortedPhotons.reserve(this->photonMap.size());

	for (it = this->photonMap.begin(), end = this->photonMap.end(); it != end; it++)
		sortedPhotons.push_back(&(*it));

	sort(sortedPhotons.begin(),sortedPhotons.end(),PointDistanceComparator(p));

	for (i = 0; i < k; i++)
		knn.push_back(sortedPhotons[i]);

	return knn;
}
