#include "Photon.hpp"
#include "Scene.hpp"

Photon(Ray& ray, double wavelength)
	: ray(ray), wavelength(wavelength)
{
}

Photon bounce(Scene& scene, int nPhotonBounce)
{
	// TODO: bounce photons

	return *this;
}

