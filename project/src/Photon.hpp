#ifndef PHOTON_HPP
#define PHOTON_HPP

#include "Ray.hpp"

/* Lower and upper visible spectrum wavelength limits */
#define VISIBLE_L_WL 390
#define VISIBLE_U_WL 750

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
