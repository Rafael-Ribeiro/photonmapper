#include "Material.hpp"

#include <math.h>

Material::Material(Color color, double roughness, double albedo, double refractance, double emittance, double n)
	: color(color), albedo(albedo), roughness(roughness), refractance(refractance), emittance(emittance), n(n)
{
}

/*
 * Reflectance according to Schlick’s approximation:
 * http://en.wikipedia.org/wiki/Schlick's_approximation
 * 
 * For a more detailed version (function implemented according to this reference's extension):
 * http://www.bramz.net/data/writings/reflection_transmission.pdf
 */
double Material::reflectance(double angle, double nFrom)
{
	/* Note that:
	 * n1 = nFrom
	 * n2 = this->n (the medium where the photon is "entering")
	 * θi = angle
	 * θf needs to be calculated if n1 > n2 and angle > TIR
	 */

	double temp, temp2;
	double R0 = (nFrom - this->n)/(nFrom + this->n);
	R0 *= R0;

	if (nFrom <= this->n)
	{
		temp = (1 - cos(angle));
		temp2 = temp * temp; /* square */

		temp2 *= temp2; /* forth */
		temp *= temp2; /* fifth */

		return (R0 + (1 - R0) * temp)*(1-this->albedo);
	} else if (angle > asin(this->n/nFrom)) /* TIR's formula: asin(n2/n1) */
	{
		/*
		 * θf is calculated using Snell's law:
		 * http://en.wikipedia.org/wiki/Snell's_law
		 */

		double theta_f = asin((sin(angle) * nFrom)/this->n);

		temp = (1 - cos(theta_f));
		temp2 = temp * temp; /* square */

		temp2 *= temp2; /* forth */
		temp *= temp2; /* fifth */

		return (R0 + (1 - R0) * temp)*(1-this->albedo);
	}

	return (1-this->albedo);
}
