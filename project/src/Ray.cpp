#include "Ray.hpp"
#include "Scene.hpp"
#include "Engine.hpp"

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
	Color self = Color(0,0,0), others = Color(0,0,0);

	Intersection intersect;
	Vector normal;
	vector<const Photon*> photons;
	vector<const Photon*>::const_iterator photon, end;
	double angle, reflectance, refractance, intensity;

	if (maxdepth == 0 || !scene.intersect(*this, intersect))
		return self;

	normal = intersect.prim->normal(intersect.point, intersect.prim->mat.roughness);

	angle = intersect.direction.angle(normal);
	if (angle > M_PI/2)
		angle = M_PI-angle;	 /* abs(-(M_PI - angle)) */

	reflectance = intersect.prim->mat.reflectance(angle, nFrom);
	refractance = intersect.prim->mat.refractance;

	if (intersect.prim->mat.albedo < 1.0)
	{
		/*if (reflectance > 0)*/
		if (true)
		{
			Ray reflectedRay;

			reflectedRay.origin = intersect.point;
			reflectedRay.direction = (this->direction - normal * 2 * this->direction.dot(normal));

			// TODO: roughness
			// reflected ray gives the axis of a cone (higher roughness -> larger cone)
			// cast N rays
			others = others + reflectedRay.getColor(scene, maxdepth-1, nFrom); // * reflectance!!!!
		}

		if (refractance > 0)
		{
			Ray refractedRay;

			/* TODO: calc ray */
			//c += refracteddRay.getColor()*(1-reflectance)*refractance;
			/* remember nFrom changes */
		}
	}

	photons = scene.getNearestPhotons(intersect.point, Engine::MAX_GATHER_DISTANCE);

	for (photon = photons.begin(), end = photons.end(); photon != end; photon++)
	{
		intensity = 1/(1 + (intersect.point - (*photon)->ray.origin).norm()/16) * Engine::EXPOSURE;
		self = self + (*photon)->color * intensity;
	}

	return (self * intersect.prim->mat.albedo + others * (1 - intersect.prim->mat.albedo)).cap();
}
