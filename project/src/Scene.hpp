#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Primitive.hpp"

using namespace std;

struct Intersection
{
	Primitive* prim;
	Point point;
	Vector direction;
};

struct Scene
{
	/*
	 * TODO:
	 * kd_tree<Photon> photons;
	 */

	vector<Primitive*> primitives;
	vector<Primitive*> lights;

	/* TODO: kd-tree */
	Intersection intersect(Ray& p) const;
};

#endif
