#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Vector.hpp"
#include "Color.hpp"

struct Material
{
	Color color;

	double roughness; /* 0..1 */
	double refractance; /* 0..1; absorvance = 1 - refractance */
	double emittance; /* 0..inf lumen/m^2 */

	/* depende de lambda TODO: http://en.wikipedia.org/wiki/Sellmeier_equation */
	double n; /* TODO: for refractions; light speed in natural unit 1 = 3*10^8 */

	Material(Color color, double roughness, double refractance, double emittance, double n);

	double reflectance(double angle);
};

#endif
