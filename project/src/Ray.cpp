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
	Color	sum = Color(0,0,0),
			colorReflection = Color(0, 0, 0),
			colorTransmission = Color(0,0,0),
			colorRefraction = Color(0,0,0),
			colorEmission = Color(0, 0, 0);

	Intersection intersect;

	Vector normal;
	vector<Photon> photons;
	vector<Photon>::const_iterator photon, end;

	Vector rayDirection;

	double reflectance, refractance, emittance, roughness;
	double angle;

	if (maxdepth == 0 || relevance < Engine::EPS || !scene.intersect(*this, intersect))
		return Color(255,0,0);

	normal = intersect.prim->normal(intersect.point);
	if (this->inside)
		normal = -normal;

	/* emittance + reflectance + refractance = 1 */
	emittance = intersect.prim->mat.emittance;
	roughness = intersect.prim->mat.roughness;

	if (this->inside)
		reflectance = scene.environment.reflectance(this->direction, normal, intersect.prim->mat);
	else
		reflectance = intersect.prim->mat.reflectance(this->direction, normal, scene.environment);

	reflectance = (1 - emittance) * reflectance;
	refractance = 1 - reflectance - emittance;

	if (reflectance > 0)
	{
		Ray specularRay;
		Color specularReflection = Color(0,0,0), diffuseReflection = Color(0,0,0);

		/* Specular reflection */

		rayDirection = intersect.prim->mat.reflectionDirection(this->direction,normal);

		specularRay.origin = intersect.point;
		specularRay.direction = rayDirection;
		specularRay.inside = this->inside;

		specularReflection = specularRay.getColor(scene, maxdepth-1, relevance * reflectance * (1 - roughness));

		/* Diffuse reflection */
		photons = scene.getNearestPhotons(intersect.point, Engine::MAX_GATHER_DISTANCE);

		for (photon = photons.begin(), end = photons.end(); photon != end; photon++)
		{	
			angle = (*photon).ray.direction.dot(normal);
			if (angle > 0 && ((intersect.point - (*photon).ray.origin).sqrd_norm() < Engine::MAX_GATHER_DISTANCE_SQRD))
				diffuseReflection = diffuseReflection + (*photon).color * Engine::EXPOSURE * angle;

		}

		diffuseReflection = diffuseReflection / (M_PI * Engine::MAX_GATHER_DISTANCE_SQRD);
		colorReflection =
				(diffuseReflection * reflectance * roughness)
				+ (specularReflection * reflectance * (1 - roughness));
	}
	
	if (refractance > 0)
	{
		Ray refractedRay;

		/* Check whether the ray is inside (= refracted ray going out) or outside (= refracted ray coming in) a primitive */
		if (this->inside)
		{
			rayDirection = scene.environment.refractionDirection(this->direction,normal,intersect.prim->mat); /* from primitive's material to scene's environment */

			/* TODO/FIXME Absorvance based on distance */
			colorRefraction = intersect.prim->mat.color;
		} else
		{
			rayDirection = intersect.prim->mat.refractionDirection(this->direction,normal,scene.environment); /* from scene's environment to primitive's material */
			colorRefraction = Color(255,255,255);
		}

		/* "original" ray must always be casted */
		refractedRay.origin = intersect.point;
		refractedRay.direction = rayDirection;
		refractedRay.inside = (this->inside ? NULL : intersect.prim); /* Set ray's relative location (inside or outside of a primitive (outside = air)) */

		colorTransmission = refractedRay.getColor(scene, maxdepth-1, relevance*refractance);
		colorRefraction = colorRefraction * colorTransmission * refractance;
	}

	if (emittance > 0)
		colorEmission = intersect.prim->mat.color * emittance;

	sum = colorReflection + colorRefraction + colorEmission;
	return sum.cap();
}
