#include "Photon.hpp"
#include "Scene.hpp"

#include <iostream>

using namespace std;

Photon::Photon()
{
}

Photon::Photon(Ray& ray, double wavelength)
	: ray(ray), wavelength(wavelength)
{
}

bool Photon::bounce(Scene& scene, unsigned int bouncesLeft, Photon& photon)
{
	Intersection intersect;

	photon = *this;

	if (!scene.intersect(this->ray, intersect))
		return false;
	
	/* TODO: reflect, refract, ... */

	photon.ray.direction = intersect.direction;
	photon.ray.origin = intersect.point;
		
	/* continue -.-" */
	if (true) /* reflect */
	{
		
	}
	
	if (bouncesLeft > 0)
		return photon.bounce(scene, bouncesLeft-1, photon);

	return true;
}

