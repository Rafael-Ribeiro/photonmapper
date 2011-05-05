#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Primitive.hpp"

struct Intersection
{
	Primitive* prim;
	Point point;
	Vector direction;
};

struct Scene
{
	Scene();

	std::vector<Photon> photonMap; /* TODO: kd_tree<Photon> photons; */

	std::vector<Primitive*> primitives;
	std::vector<Primitive*> lights;

	/* TODO: kd-tree */
	bool intersect(Ray& p, Intersection& intersect) const;
	void buildPhotonMap(unsigned int nPhotons, unsigned int nPhotonBounce);
};

#endif
