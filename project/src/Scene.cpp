#include <float.h>
#include <iostream>
#include <algorithm>

#include "Scene.hpp"
#include "Primitive.hpp"

#include "kdtree++/kdtree.hpp"

using namespace std;

Scene::Scene()
	 : nPhotons(0), environment(Material(Color(0xFF,0xFF,0xFF), 0.0, 0.0, 0.0, N_AIR))
{
}

struct PointDistanceComparator
{
	Point p;

	PointDistanceComparator(const Point& p)
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

	sum = 0;
	for (i = 0; i < this->lights.size(); i++)
		sum += this->lights[i]->area() * this->lights[i]->mat.emittance;

	for (i = 0; i < this->lights.size(); i++)
	{
		n = this->lights[i]->area() * this->lights[i]->mat.emittance*nPhotons/sum;
	
		for (j = 0; j < n; j++)
			this->lights[i]->randomPhoton().bounce(*this, nPhotonBounce, photon);	
	}

	cerr << "No. of photons: " << this->nPhotons << endl; 
	this->photonMap.optimise();
}

void Scene::storePhoton(Photon photon)
{
	this->photonMap.insert(photon);
	this->nPhotons++;
}

/*
 * TODO: getKNearestPhotons using KD-Tree
 * 
 * getKNearestPhotons is simply iterating the scene's photonMap
 * (which is a vector at the moment) and sorting Photon* by their distance to p,
 * to return the first k Photon* of the sort
 */
vector<Photon> Scene::getNearestPhotons(const Point& p, double distance) const
{
	vector<Photon> photons;
	Photon photon(Ray(p, Vector(0,0,0)), Color(0,0,0));

	this->photonMap.find_within_range(photon, distance, back_insert_iterator<vector<Photon> >(photons));

	return photons;
}
