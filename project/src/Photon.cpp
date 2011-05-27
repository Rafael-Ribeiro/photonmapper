#include "Photon.hpp"
#include "Scene.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

Photon::Photon()
{
}

Photon::Photon(Ray& ray, Color color) /* double wavelength */
	: ray(ray), color(color)
{
}

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

/* nFrom refers to the light speed in the medium from where the photon comes */
void Photon::bounce(Scene& scene, unsigned int bouncesLeft, Photon& photon, double nFrom)
{
	if (bouncesLeft == 0)
	{
		photon = *this;
		return;
	}

	Intersection intersect;
	Vector normal;

	photon = *this;

	if (!scene.intersect(this->ray, intersect))
		return;

	/* photon intersects on object */
	photon.color.r = min(photon.color.r, intersect.prim->mat.color.r);
	photon.color.g = min(photon.color.g, intersect.prim->mat.color.g);
	photon.color.b = min(photon.color.b, intersect.prim->mat.color.b);

	/* store this photon */
	scene.photonMap.push_back(photon);

	/* bounce it arround */		
	 /* reflectance depends on the angle between photon's ray and the primitive's normal on the intersection point  */
	double refl = intersect.prim->mat.reflectance(intersect.direction.angle(intersect.prim->normal(intersect.point)), nFrom);

	photon.ray.origin = intersect.point;
	if (random() < refl)
	{
		/* reflection http://en.wikipedia.org/wiki/Reflection_%28mathematics%29 */
		normal = intersect.prim->normal(intersect.point);
		photon.ray.direction = (photon.ray.direction - normal*2*photon.ray.direction.dot(normal)).normalized();
	
		photon.bounce(scene, bouncesLeft-1, photon, nFrom);
	} else if (true)
	{
		/* TODO: refraction */
	}
}
