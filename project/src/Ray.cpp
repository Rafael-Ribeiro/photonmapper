#include "Ray.hpp"
#include "Scene.hpp"
#include "Engine.hpp"

#include <cmath>
#include <iostream>
#include <iomanip>

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
	int i;

	Color sum = Color(0,0,0), self = Color(0, 0, 0), others = Color(0, 0, 0), transmittanceColor = Color(255,255,255);

	Intersection intersect;

	Vector normal;
	Vector rayDirection;

	vector<const Photon*> photons;
	vector<const Photon*>::const_iterator photon, end;

	double reflectance, refractance, absorvance, emittance;
	double angle, intensity, distance;

	int diffuseRays;

	if (maxdepth == 0 || relevance < Engine::EPS || !scene.intersect(*this, intersect))
		return Color(0,0,0);
	//	return self;

	normal = intersect.prim->normal(intersect.point);
	if (this->inside)
		normal = -normal;

	/* emitance + absorvance + reflectance + refractance = 1 */
	emittance = intersect.prim->mat.emittance;
	absorvance = intersect.prim->mat.absorvance*(1-emittance);

	if (this->inside)
		reflectance = scene.environment.reflectance(this->direction, normal, intersect.prim->mat);
	else
		reflectance = intersect.prim->mat.reflectance(this->direction, normal, scene.environment);

	refractance = (1-absorvance - emittance)*(1-reflectance);
	reflectance = (1-absorvance - emittance)*reflectance;

	diffuseRays = max(1, (int) (intersect.prim->mat.roughness * Engine::MAX_DIFFUSE_RAYS)); /* to guarantee that the original ray is casted */
	
	if (reflectance > 0)
	{
		Ray reflectedRay;
		Color avgReflectionColor;

		rayDirection = intersect.prim->mat.reflectionDirection(this->direction,normal);

		/* "original" ray must always be casted */
		reflectedRay.origin = intersect.point;
		reflectedRay.direction = rayDirection;
		reflectedRay.inside = this->inside;

		avgReflectionColor = reflectedRay.getColor(scene, maxdepth-1, relevance * reflectance / diffuseRays);

		if (maxdepth == Engine::MAX_RAY_BOUNCE)
		{
			for (i = 1; i < diffuseRays; i++)
			{
				reflectedRay.origin = intersect.point;
				reflectedRay.direction = rayDirection.noise(intersect.prim->mat.roughness);
				reflectedRay.inside = this->inside;

				avgReflectionColor = avgReflectionColor + reflectedRay.getColor(scene, 3, relevance * reflectance / diffuseRays);
			}

			if (diffuseRays > 1)
				avgReflectionColor = avgReflectionColor / diffuseRays;
		}

		others = others + avgReflectionColor * reflectance;
	}
	
	if (refractance > 0)
	{
		Ray refractedRay;
		Color avgRefractionColor;

		/* Check whether the ray is inside (= refracted ray going out) or outside (= refracted ray coming in) a primitive */
		if (this->inside)
		{
			rayDirection = scene.environment.refractionDirection(this->direction,normal,intersect.prim->mat); /* from primitive's material to scene's environment */

			/* TODO/FIXME Absorvance based on distance */
			transmittanceColor.r = intersect.prim->mat.color.r;
			transmittanceColor.g = intersect.prim->mat.color.g;
			transmittanceColor.b = intersect.prim->mat.color.b;
		} else
			rayDirection = intersect.prim->mat.refractionDirection(this->direction,normal,scene.environment); /* from scene's environment to primitive's material */

		/* "original" ray must always be casted */
		refractedRay.origin = intersect.point;
		refractedRay.direction = rayDirection;
		refractedRay.inside = (this->inside ? NULL : intersect.prim); /* Set ray's relative location (inside or outside of a primitive (outside = air)) */

		avgRefractionColor = refractedRay.getColor(scene, maxdepth-1, relevance*refractance);

		if (maxdepth == Engine::MAX_RAY_BOUNCE)
		{
			for (i = 1; i < diffuseRays; i++)
			{
				refractedRay.origin = intersect.point;
				refractedRay.direction = rayDirection.noise(intersect.prim->mat.roughness);
				refractedRay.inside = (this->inside ? NULL : intersect.prim); /* Set ray's relative location (inside or outside of a primitive (outside = air)) */

				avgRefractionColor = avgRefractionColor + refractedRay.getColor(scene, 3, relevance*refractance);
			}

			if (diffuseRays > 1)
				avgRefractionColor = avgRefractionColor / diffuseRays;
		}

		others = others + avgRefractionColor * refractance * transmittanceColor;
	}

	if (absorvance > 0)
	{
		photons = scene.getNearestPhotons(intersect.point, Engine::MAX_GATHER_DISTANCE);

		for (photon = photons.begin(), end = photons.end(); photon != end; photon++)
		{	
			/* TODO: maybe this should take into account not only the direction of the photon but its position too */
			/* TODO: make this take into account rougness for angle attenuation*/
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
