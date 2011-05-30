#include "Ray.hpp"
#include "Scene.hpp"
#include "Engine.hpp"

#include <cmath>
#include <iostream>

using namespace std;

Ray::Ray()
{
}

Ray::Ray(const Point& origin, const Vector& direction)
	: origin(origin), direction(direction), inside(NULL)
{
}

Color Ray::getColor(const Scene& scene, int maxdepth, double nFrom, double relevance) const
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

	normal = intersect.prim->normal(intersect.point, intersect.prim->mat.roughness);

	angle = intersect.direction.angle(normal);
	if (angle > M_PI/2)
		angle = M_PI-angle;	 /* abs(-(M_PI - angle)) */
	
	/* emitance + absorvance + reflectance + refractance = 1 */
	emittance = intersect.prim->mat.emittance;
	absorvance = intersect.prim->mat.absorvance*(1-emittance);
	reflectance = intersect.prim->mat.reflectance(angle, nFrom);
	refractance = (1-absorvance)*(1-reflectance);
	reflectance = (1-absorvance)*reflectance;

	if (reflectance > 0)
	{
		Ray reflectedRay;

		reflectedRay.origin = intersect.point;
		reflectedRay.direction = (this->direction - normal * 2 * this->direction.dot(normal));

		// TODO: roughness
		// reflected ray gives the axis of a cone (higher roughness -> larger cone)
		// cast N rays
		others = others + reflectedRay.getColor(scene, maxdepth-1, nFrom, relevance*reflectance) * reflectance;
	}

	if (refractance > 0)
	{
		/*
		 * Refraction implemented according to:
		 * http://www.bramz.net/data/writings/reflection_transmission.pdf
		 */

		Ray refractedRay;
		double nTo;
		double n, cosI, sinT2, cosT;

		/* Set ray's relative location (inside or outside of a primitive (outside = air)) */
		refractedRay.inside = (this->inside ? NULL : intersect.prim);

		/* If ray is "going to be" inside of a primitive, that primitive's material n is the nTo; else we assume it's air */
		nTo = (refractedRay.inside ? refractedRay.inside->mat.n : N_AIR);

		refractedRay.origin = intersect.point;

		n = nFrom / nTo;
		cosI = -normal.dot(this->direction);
		sinT2 = n * n * (1.0 - cosI * cosI);

		if (sinT2 <= 1.0) /* not inside TIR's range: this ray may be casted */
		{
			cosT = sqrt(1.0 - sinT2);

			refractedRay.direction = (this->direction * n + normal * (n * cosI - cosT)).normalized();

			/*
			 * TODO:
			 * add roughness noise
			 */
			others = others + refractedRay.getColor(scene, maxdepth-1, nTo, relevance*refractance) * refractance;
		} else
			cerr << "Bode: " << sinT2 << " " << cosI << " refractance " << refractance << endl
				 << "Normal: " << normal.x << " " << normal.y << " " << normal.z << " | " << normal.norm() << endl
				 << "Direction: " << this->direction.x << " " << this->direction.y << " " << this->direction.z << " | " << this->direction.norm() << endl
				 << "Intersection: " << intersect.point.x << " " << intersect.point.y << " " << intersect.point.z << endl
				 << "nFrom: " << nFrom << " nTo: " << nTo << " angle: " << angle << " | " << acos(normal.dot(this->direction)) << endl;
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
