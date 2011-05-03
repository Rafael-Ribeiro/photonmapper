#include "Photon.hpp"
#include "Scene.hpp"

Photon::Photon(Ray& ray, double wavelength)
	: ray(ray), wavelength(wavelength)
{
}

Photon Photon::bounce(Scene& scene, int nPhotonBounce)
{
	// TODO: bounce photons

	return *this;
}

