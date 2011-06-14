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
void Photon::bounce(Scene& scene, unsigned int bouncesLeft, Photon& photon)
{
	Intersection intersect;
	Vector normal;
	double reflectance, refractance, absorvance;
	double r;

	photon = *this;

	if (photon.color == Color(0,0,0))
		return;

	if (bouncesLeft == 0 || !scene.intersect(this->ray, intersect))
		return;

	normal = intersect.prim->normal(intersect.point);
	if (this->ray.inside)
		normal = -normal;

	absorvance = intersect.prim->mat.absorvance;

	if (this->ray.inside)
		reflectance = scene.environment.reflectance(this->ray.direction, normal, intersect.prim->mat);
	else
		reflectance = intersect.prim->mat.reflectance(this->ray.direction, normal, scene.environment);

	reflectance = (1 - absorvance) * reflectance;
	refractance = (1 - absorvance) * (1 - reflectance);

	r = random01();
	
	photon.ray.origin = intersect.point;

	if (r < absorvance)
	{
		/* photon is absorved (an then emited) by the object */
		photon.color.r = min(this->color.r, intersect.prim->mat.color.r);
		photon.color.g = min(this->color.g, intersect.prim->mat.color.g);
		photon.color.b = min(this->color.b, intersect.prim->mat.color.b);

		/* store this photon */
		photon.ray.direction = -normal;
		scene.photonMap.push_back(photon);

		photon.ray.direction = normal.noise(intersect.prim->mat.roughness);
	} else if (r < absorvance + reflectance) /* random01() < refl */
	{
		photon.ray.direction = intersect.prim->mat.reflectionDirection(this->ray.direction, normal);
	} else
	{
		/* Check whether the ray is inside (= refracted ray going out) or outside (= refracted ray coming in) a primitive */
		if (this->ray.inside)
			this->ray.direction = scene.environment.refractionDirection(this->ray.direction, normal, intersect.prim->mat); /* from primitive's material to scene's environment */
		else
			this->ray.direction = intersect.prim->mat.refractionDirection(this->ray.direction, normal, scene.environment); /* from scene's environment to primitive's material */

		/* Set ray's relative location (inside or outside of a primitive (outside = air)) */
		this->ray.inside = (this->ray.inside ? NULL : intersect.prim);
	}

	photon.bounce(scene, bouncesLeft-1, photon);
}
