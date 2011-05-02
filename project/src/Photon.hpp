#ifndef PHOTON_HPP
#define PHOTON_HPP

#include "Ray.hpp"
#include "Scene.hpp"

struct Photon
{
	Ray ray;
	double wavelength;

	Photon(Ray& ray, double wavelength);
	Photon bounce(Scene& scene, int bounces);
};

#endif
