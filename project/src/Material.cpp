#include "Material.hpp"

Material::Material(Color color, double roughness, double refractance, double emittance, double n)
	: color(color), roughness(roughness), refractance(refractance), emittance(emittance), n(n)
{
}

double Material::reflectance(double angle, double nFrom)
{
	/* TODO: reflectance */
	/* based on incident angle AND roughness -> calc reflectance */
	return 1.0;
}
