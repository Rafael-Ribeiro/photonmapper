#include "Photon.hpp"
#include "Scene.hpp"
#include "utils.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

Photon::Photon()
{
}

Photon::Photon(const Ray& ray, const Color& color) /* double wavelength */
	: ray(ray), color(color)
{
}

/* nFrom refers to the light speed in the medium from where the photon comes */
void Photon::bounce(Scene& scene, unsigned int bouncesLeft, Photon& photon, double nFrom)
{
	Intersection intersect;
	Vector normal;
	double angle;
	double reflectance, refractance, absorvance;
	double r;

	photon = *this;

	if (photon.color == Color(0,0,0))
		return;

	if (bouncesLeft == 0 || !scene.intersect(this->ray, intersect))
		return;

	normal = intersect.prim->normal(intersect.point).noise(intersect.prim->mat.roughness);
	angle = intersect.direction.angle(normal);

	/* absorvance + reflectance + refractance = 1 */
	absorvance = intersect.prim->mat.absorvance;
	reflectance = intersect.prim->mat.reflectance(angle, nFrom);
	refractance = (1-absorvance)*(1-reflectance);
	reflectance = (1-absorvance)*reflectance;

	r = random01();
	if (r < absorvance)
	{
		/* photon is absorved (an then emited) by the object */
		photon.ray.origin = intersect.point;	
		photon.color.r = min(this->color.r, intersect.prim->mat.color.r);
		photon.color.g = min(this->color.g, intersect.prim->mat.color.g);
		photon.color.b = min(this->color.b, intersect.prim->mat.color.b);

		/* store this photon */
		photon.ray.direction = -normal;
		scene.photonMap.push_back(photon);

		photon.ray.direction = normal;

	} else if (r < absorvance + reflectance) /* random01() < refl */
	{
		/* reflection http://en.wikipedia.org/wiki/Reflection_%28mathematics%29 */
		photon.ray.direction = (photon.ray.direction - normal*2*photon.ray.direction.dot(normal)).normalized();
	} else
	{
		/* TODO: refraction */
		photon.ray.direction = (photon.ray.direction - normal*2*photon.ray.direction.dot(normal)).normalized();
	}

	photon.bounce(scene, bouncesLeft-1, photon, nFrom);
}
