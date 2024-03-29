#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Vector.hpp"
#include "Color.hpp"
#include "Ray.hpp"

#include "jsonbox/inc/JsonBox.h"

struct Material
{
	string name;

	Color color;

	double absorvance; /* 0..1; */
	double roughness; /* 0..1: noise percentage to be added to the reflection angle */
	double refractance; /* 0..1; */
	double emittance; /* 0..inf lumen/m^2 */

	double n; /* for refractions; light speed in natural unit 1 = 3*10^8 */

	Material();
	Material(string name, const Color& color, double roughness, double absorvance, double emittance, double n);

	static bool parse(const JsonBox::Value &materialVal, Material &material);

	double reflectance(const Vector &direction, const Vector &normal, const Material &fromMaterial) const;
	Vector reflectionDirection(const Vector &direction, const Vector &normal) const;
	Vector refractionDirection(const Vector &direction, const Vector &normal, const Material &fromMaterial) const;
};

#endif
