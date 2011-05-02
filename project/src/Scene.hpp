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
	/*
	 * TODO:
	 * kd_tree<Photon> photons;
	 */
	Scene();

	std::vector<Primitive*> primitives;
	std::vector<Primitive*> lights;

	/* TODO: kd-tree */
	Intersection intersect(Ray& p) const;
};

#endif
