#include "Photon.hpp"
#include "Scene.hpp"

#include <iostream>

using namespace std;

Photon::Photon()
{
}

Photon::Photon(Ray& ray, double wavelength)
	: ray(ray), wavelength(wavelength)
{
}

/* nFrom refers to the light speed in the medium from where the photon comes */
bool Photon::bounce(Scene& scene, unsigned int bouncesLeft, Photon& photon, double nFrom)
{
	Intersection intersect;
	Vector normal;

	photon = *this;

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
	double refl = intersect.prim->mat.reflectance(intersect.direction.angle(intersect.prim->normal(intersect.point)),nFrom);
		
	/* continue -.-" */
	photon.ray.direction = intersect.direction;
	photon.ray.origin = intersect.point;
		
	/* continue -.-" */
	if (true) /* reflection: http://en.wikipedia.org/wiki/Reflection_%28mathematics%29 */
	{
		normal = intersect.prim.normal(intersect.point);
		photon.ray.direction -= 2*photon.ray.direction.dot(normal)*normal;
	}
	
	if (bouncesLeft > 0)
		return photon.bounce(scene, bouncesLeft-1, photon, nFrom);

	return true;
}

