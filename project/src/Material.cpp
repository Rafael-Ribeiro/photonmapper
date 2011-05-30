#include "Material.hpp"

#include <math.h>
#include <iostream>

using namespace std;

Material::Material(const Color& color, double roughness, double absorvance, double emittance, double n)
	: color(color), absorvance(absorvance), roughness(roughness), emittance(emittance), n(n)
{
}

double Material::reflectance(const Vector &direction, const Vector &normal, const Material &fromMaterial) const
{
	/*
	 * Reflectance according to http://www.bramz.net/data/writings/reflection_transmission.pdf
	 */

	double n = fromMaterial.n / this->n;
	double cosI = -(normal.dot(direction));
	double sinT2 = n * n * (1.0 - cosI * cosI);

	if (sinT2 > 1.0) /* inside TIR */
		return 1.0;

	double cosT = sqrt(1.0 - sinT2);
	double rOrth = (fromMaterial.n * cosI - this->n * cosT) / (fromMaterial.n * cosI + this->n * cosT);
	double rPar = (this->n * cosI - fromMaterial.n * cosT) / (this->n * cosI + fromMaterial.n * cosT);

	return (rOrth * rOrth + rPar * rPar) / 2.0;
}

Vector Material::reflectionDirection(const Vector &direction, const Vector &normal) const
{
	return direction - normal * 2 * direction.dot(normal);
}

Vector Material::refractionDirection(const Vector &direction, const Vector &normal, const Material &fromMaterial) const
{
	/*
	 * Refraction direction according to http://www.bramz.net/data/writings/reflection_transmission.pdf
	 */

	double n = fromMaterial.n / this->n;
	double cosI = -normal.dot(direction);
	double sinT2 = n * n * (1.0 - cosI * cosI);

	if (sinT2 > 1.0) /* inside TIR's range: 100% reflection MUST occur */
		cerr << "ERROR: Invalid refraction (check Material.cpp)" << endl;

	double cosT = sqrt(1.0 - sinT2);

	return (direction * n + normal * (n * cosI - cosT)).normalized();
}
