#include "Ray.hpp"
#include "Scene.hpp"
#include "Engine.hpp"

#include <cmath>
#include <iostream>

using namespace std;

Ray::Ray()
	: inside(NULL)
{
}

Ray::Ray(const Point& origin, const Vector& direction)
	: origin(origin), direction(direction), inside(NULL) /* Ray begins outside of a primitive (on scene's environment) by default */
{
}

Color Ray::getColor(const Scene& scene, int maxdepth, double relevance) const
{
	Color sum = Color(0,0,0), self = Color(0, 0, 0), others = Color(0, 0, 0);

	Intersection intersect;
	Vector normal;
	vector<const Photon*> photons;
	vector<const Photon*>::const_iterator photon, end;
	double reflectance, refractance, absorvance, emittance;
	double angle, intensity, distance;

	if (maxdepth == 0 || relevance < Engine::EPS || !scene.intersect(*this, intersect))
		return self;

	normal = intersect.prim->normal(intersect.point).noise(intersect.prim->mat.roughness);
	if (this->inside)
		normal = -normal;

	/* emitance + absorvance + reflectance + refractance = 1 */
	emittance = intersect.prim->mat.emittance;
	absorvance = intersect.prim->mat.absorvance*(1-emittance);

	if (this->inside)
		reflectance = scene.environment.reflectance(this->direction, normal, intersect.prim->mat);
	else
		reflectance = intersect.prim->mat.reflectance(this->direction, normal, scene.environment);

	refractance = (1-absorvance)*(1-reflectance);
	reflectance = (1-absorvance)*reflectance;

	if (reflectance > 0)
	{
		Ray reflectedRay;

		reflectedRay.origin = intersect.point;
		reflectedRay.direction = intersect.prim->mat.reflectionDirection(this->direction,normal);
		reflectedRay.inside = this->inside;

		// TODO: roughness
		// reflected ray gives the axis of a cone (higher roughness -> larger cone)
		// cast N rays
		others = others + reflectedRay.getColor(scene, maxdepth-1, relevance*reflectance) * reflectance;
	}

	if (refractance > 0)
	{
		Ray refractedRay;

		refractedRay.origin = intersect.point;

		/* Set ray's relative location (inside or outside of a primitive (outside = air)) */
		refractedRay.inside = (this->inside ? NULL : intersect.prim);

		/* Check whether the ray is inside (= refracted ray going out) or outside (= refracted ray coming in) a primitive */
		if (this->inside)
			refractedRay.direction = scene.environment.refractionDirection(this->direction,normal,intersect.prim->mat); /* from primitive's material to scene's environment */
		else
			refractedRay.direction = intersect.prim->mat.refractionDirection(this->direction,normal,scene.environment); /* from scene's environment to primitive's material */

		others = others + refractedRay.getColor(scene, maxdepth-1, relevance*refractance) * refractance;
	}

	if (absorvance > 0)
	{
		photons = scene.getNearestPhotons(intersect.point, Engine::MAX_GATHER_DISTANCE);

		for (photon = photons.begin(), end = photons.end(); photon != end; photon++)
		{	
			/* TODO: maybe this should take into account not only the direction of the photon but its position too */
			angle = (*photon)->ray.direction.dot(normal);
			if (angle < 0)
			{
				distance = (intersect.point - (*photon)->ray.origin).norm();
				intensity = Engine::EXPOSURE /
				(
					Engine::CONSTANT_LIGHT_ATTENUATION +
					distance * Engine::LINEAR_LIGHT_ATTENUATION +
					distance * distance * Engine::QUADRATIC_LIGHT_ATTENUATION
				);
				self = self + (*photon)->color * intensity * (-angle);
			}
		}

		self = self * absorvance;
	}

	if (emittance > 0)
		self = self + intersect.prim->mat.color * emittance;

	sum = self + others;

	return sum.cap();
}
