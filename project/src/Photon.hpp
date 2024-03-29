#ifndef PHOTON_HPP
#define PHOTON_HPP

#include <stdlib.h>

#include "Ray.hpp"
#include "Color.hpp"

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
	Color color;

	Photon();
	Photon(const Ray& ray, const Color& color);
	void bounce(Scene& scene, unsigned int bouncesLeft, Photon& photon);

	/* kd-tree */
	typedef double value_type;
 	value_type operator[] (size_t n) const;
	double distance_to(Photon const& x) const;
};

#endif
