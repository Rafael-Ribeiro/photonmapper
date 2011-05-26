#include "Photon.hpp"
#include "Scene.hpp"

Photon::Photon()
{
}

Photon::Photon(Ray& ray, double wavelength)
	: ray(ray), wavelength(wavelength)
{
}

/* nMedium refers to the light speed in the medium from where the photon comes */
bool Photon::bounce(Scene& scene, unsigned int bouncesLeft, Photon& photon, double nMedium)
{
	Intersection intersect;

	photon = *this;	
	return true;

	if (!scene.intersect(this->ray, intersect))
		return false;

	/* TODO: reflect, refract, ... */
	/*
		pseudo-algorithm for ray tracing:

		based on incident angle AND roughness -> calc reflectance
		if not reflected
			if refractance
				refract
			else
				absorve
		else
			reflect
	*/

	 /* reflectance depends on the angle between photon's ray and the primitive's normal on the intersection point  */
	double refl = intersect.prim->mat.reflectance(intersect.direction.angle(intersect.prim->normal(intersect.point)),nMedium);

	

	return true;
}

