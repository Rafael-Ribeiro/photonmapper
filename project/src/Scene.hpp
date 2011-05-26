#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Primitive.hpp"

#define N_AIR ((double)1.0) /* light of speed in the air is approximately the same as in vacuum */

using namespace std;

struct Intersection
{
	Primitive* prim;
	Point point;
	Vector direction;
};

struct Scene
{
	Scene();

	vector<Photon> photonMap; /* TODO: kd_tree<Photon> photons; */

	vector<Primitive*> primitives;
	vector<Primitive*> lights;

	/* TODO: kd-tree */
	bool intersect(const Ray& p, Intersection& intersect) const;
	void buildPhotonMap(int nPhotons, int nPhotonBounce);
	vector<const Photon*> getKNearestPhotons(Point p, int k) const;
};

#endif
