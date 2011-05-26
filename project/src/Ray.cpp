#include "Ray.hpp"
#include "Scene.hpp"

#include <cmath>
#include <iostream>

using namespace std;

Ray::Ray()
{
}

Ray::Ray(Point origin, Vector direction)
	: origin(origin), direction(direction)
{
}

Color Ray::getColor(const Scene& scene, int maxdepth, double nFrom) const
{
	Color c = Color(0,0,0);
	Intersection intersect;
	Vector normal;

	if (maxdepth == 0 || !scene.intersect(*this, intersect))
		return c;

	double angle = intersect.direction.angle(intersect.prim->normal(intersect.point));
	if (angle > M_PI/2)
		angle = M_PI-angle;	 /* abs(-(M_PI - angle)) */

	double reflectance = intersect.prim->mat.reflectance(angle, nFrom);
	double refractance = intersect.prim->mat.refractance;

	if (reflectance > 0)
	{
		Ray reflectedRay;

		normal = intersect.prim->normal(reflectedRay.origin);
		reflectedRay.origin = intersect.point;
		reflectedRay.direction = this->direction - normal * 2 * this->direction.dot(normal);

		c = c + reflectedRay.getColor(scene, maxdepth-1, nFrom) * reflectance;
	}

	if (refractance > 0)
	{
		Ray refractedRay;

		/* TODO: calc ray */
		//c += refracteddRay.getColor()*(1-reflectance)*refractance;
		/* remember nFrom changes */
	}

	/* TODO: instead of material color, get KNN photons and use the avereged color * irrandiance */

	c = c + intersect.prim->mat.color*(1-reflectance)*(1-refractance);

	return c;
}
