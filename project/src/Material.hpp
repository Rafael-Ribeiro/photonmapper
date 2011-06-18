#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Vector.hpp"
#include "Color.hpp"
#include "Ray.hpp"

struct Material
{
	Color color;

	double absorvance; /* 0..1; */
	double roughness; /* 0..1: noise percentage to be added to the reflection angle */
	double refractance; /* 0..1; */
	double emittance; /* 0..inf lumen/m^2 */

	/* depende de lambda TODO: http://en.wikipedia.org/wiki/Sellmeier_equation */
	double n; /* TODO: for refractions; light speed in natural unit 1 = 3*10^8 */

	Material();
	Material(const Color& color, double roughness, double absorvance, double emittance, double n);

	double reflectance(const Vector &direction, const Vector &normal, const Material &fromMaterial) const;
	Vector reflectionDirection(const Vector &direction, const Vector &normal) const;
	Vector refractionDirection(const Vector &direction, const Vector &normal, const Material &fromMaterial) const;
};

#endif
