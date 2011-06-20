#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Primitive.hpp"
#include "Material.hpp"

#include "kdtree++/kdtree.hpp"
#include "jsonbox/inc/JsonBox.h"

#define N_AIR ((double)1.0) /* light of speed in the air is approximately the same as in vacuum */

using namespace std;

/* kd-tree implementation */
typedef KDTree::KDTree<3, Photon> tree_type;

struct Intersection
{
	Primitive *prim;
	Point point;
	Vector direction;
};

struct Scene
{
	Scene();

	tree_type photonMap;
	int nPhotons;

	vector<Primitive*> primitives;
	vector<Primitive*> lights;

	Material environment;

	bool parse(const JsonBox::Value &sceneVal);
	/* TODO: kd-tree */
	bool intersect(const Ray& p, Intersection& intersect) const;
	void buildPhotonMap(int nPhotons, int nPhotonBounce);
	void storePhoton(Photon photon);
	vector<Photon> getNearestPhotons(const Point& p, double distance) const;
};

#endif
