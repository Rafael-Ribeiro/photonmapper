#include <iostream>
#include <float.h>
#include <algorithm>
#include <map>

#include "Scene.hpp"
#include "Primitive.hpp"

#include "kdtree++/kdtree.hpp"
#include "jsonbox/inc/JsonBox.h"

using namespace std;

Scene::Scene()
	 : nPhotons(0), environment(Material("surroundingEnvironment", Color(0xFF,0xFF,0xFF), 0.0, 0.0, 0.0, N_AIR))
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

bool Scene::parse(const JsonBox::Value &sceneVal)
{
	JsonBox::Object sceneObj;

	JsonBox::Value materialsVal;
	JsonBox::Array materialsArr;

	JsonBox::Value lightsVal;
	JsonBox::Array lightsArr;

	JsonBox::Value primitivesVal;
	JsonBox::Array primitivesArr;

	JsonBox::Value tempVal;

	map<string,Material> materials;

	JsonBox::Array::const_iterator it;

	Material material;
	Primitive *primitive;


	if (sceneVal.isNull())
	{
		cerr << "Error: description file doesn't implement Scene." << endl;
		return false;
	}

	sceneObj = sceneVal.getObject();

	materialsVal = sceneObj["Materials"];
	lightsVal = sceneObj["Lights"];
	primitivesVal = sceneObj["Primitives"];
	if (!materialsVal.isArray() || !lightsVal.isArray() || !primitivesVal.isArray())
	{
		cerr << "Error: description file must implement Scene:Materials, Scene:Lights and Scene:Primitives (maybe not Arrays?)." << endl;
		return false;
	}

	materialsArr = materialsVal.getArray();
	lightsArr = lightsVal.getArray();
	primitivesArr = primitivesVal.getArray();

	if (materialsArr.size() == 0 || lightsArr.size() == 0 || primitivesArr.size() == 0)
	{
		cerr << "Error: description file must have at least one Material, one Light and one Primitive." << endl;
		return false;
	}


	/* Materials */
	for (it = materialsArr.begin(); it != materialsArr.end(); it++)
	{
		if (!Material::parse(*it, material))
			break;

		materials[material.name] = material;
	}

	if (it != materialsArr.end())
	{
		cerr << "Error: invalid Material format." << endl;
		return false;
	}


	/* Lights */
	for (it = lightsArr.begin(); it != lightsArr.end(); it++)
	{
		primitive = Primitive::parse(*it, materials);
		if (!primitive)
			break;

		JsonBox::Object primitiveObject = it->getObject();

		if (primitiveObject["collidable"].isBoolean() && primitiveObject["collidable"].getBoolean() == true)
			this->lights.push_back(primitive);
		else
		{
			cerr << "Error: Light object missing collidable parameter." << endl;
			break;
		}

		this->primitives.push_back(primitive);
	}

	if (it != lightsArr.end())
	{
		cerr << "Error: invalid Light format." << endl;
		return false;
	}


	/* Primitives */
	for (it = primitivesArr.begin(); it != primitivesArr.end(); it++)
	{
		primitive = Primitive::parse(*it, materials);
		if (!primitive)
			break;

		this->primitives.push_back(primitive);
	}

	if (it != primitivesArr.end())
	{
		cerr << "Error: invalid Primitive format." << endl;
		return false;
	}

	return true;
}

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

	if (this->nPhotons != 0)
		return;

	cerr << "Building Photon Map" << endl;

	sum = 0;
	for (i = 0; i < this->lights.size(); i++)
		sum += this->lights[i]->area() * this->lights[i]->mat.emittance;

	for (i = 0; i < this->lights.size(); i++)
	{
		n = this->lights[i]->area() * this->lights[i]->mat.emittance*nPhotons/sum;

		for (j = 0; j < n; j++)
			this->lights[i]->randomPhoton().bounce(*this, nPhotonBounce, photon);	
	}

	this->photonMap.optimise();
	cerr << "No. of photons: " << this->nPhotons << endl;
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
