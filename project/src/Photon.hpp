#ifndef PHOTON_HPP
#define PHOTON_HPP

#include "Ray.hpp"

 /*
  * FIXME:
  * Scene forward declaration;
  * remove this later if possible (since dependencies may change)
  */
struct Scene;

struct Photon
{
	Ray ray;
	double wavelength;

	Photon(Ray& ray, double wavelength);
	Photon bounce(Scene& scene, int bounces);
};

#endif
