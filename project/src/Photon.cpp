#include "Photon.hpp"
#include "Scene.hpp"

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
	
	if (!scene.intersect(this->ray, intersect))
		return false;

	/* TODO: reflect, refract, ... */

	return true;
}

