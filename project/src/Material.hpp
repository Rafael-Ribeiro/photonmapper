#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Vector.hpp"
#include "Color.hpp"

/*
	pseudo-algorithm for ray tracing:

	based on incident angle AND roughness -> calc reflectance
	if not reflected
		if transmitted
			refract
		else
			absorve
	else
		reflect
*/

struct Material
{
	Color color;

	double roughness; /* 0..1 */
	double transmittance; /* 0..1; refractance = 1 - transmittance */
	double emittance; /* 0..inf lumen/m^2 */

	double c; /* TODO: for refractions; light speed in natural unit 1 = 3*10^8 */
};

#endif
