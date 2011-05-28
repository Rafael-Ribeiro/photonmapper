#include "Photon.hpp"
#include "Scene.hpp"
#include "utils.hpp"

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
	Intersection intersect;
	Vector normal;

	photon = *this;

	/* store this photon */
	scene.photonMap.push_back(photon);

	if (bouncesLeft == 0 || !scene.intersect(this->ray, intersect))
		return;

	normal = intersect.prim->normal(intersect.point, intersect.prim->mat.roughness);

	/* reflectance depends on the angle between photon's ray and the primitive's normal on the intersection point  */
	double refl = intersect.prim->mat.reflectance(intersect.direction.angle(normal), nFrom);

	photon.ray.origin = intersect.point;	
	photon.color.r = min(this->color.r, intersect.prim->mat.color.r);
	photon.color.g = min(this->color.g, intersect.prim->mat.color.g);
	photon.color.b = min(this->color.b, intersect.prim->mat.color.b);

	if (random01() < intersect.prim->mat.absorvance)
	{
		/* photon is absorved (an then emited) by the object */
		photon.ray.direction = normal;
	} else if (true)
	{
		/* reflection http://en.wikipedia.org/wiki/Reflection_%28mathematics%29 */
		photon.ray.direction = (photon.ray.direction - normal*2*photon.ray.direction.dot(normal)).normalized();

	} else if (true)
	{
		/* TODO: refraction */
	}

	photon.bounce(scene, bouncesLeft-1, photon, nFrom);
}
