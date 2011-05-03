#include <cmath>

#include "Sphere.hpp"
#include "Vector.hpp"
#include "Photon.hpp"
#include "utils.hpp"

Sphere::Sphere(Material mat, Vector center, double radius)
	: Primitive(mat), center(center), radius(radius)
{
}

Vector Sphere::intersect(Ray r)
{
	/*
	 * TODO
	 * http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
	 * for mathematical formulation and C++ code snippet
	 *
	 * FIXME
	 * Shouldn't the return type be a Point? (I know it's a typedef)
	 * Or maybe a boolean (returning the intersection through reference modification)
	 */
	return Vector(0.0, 0.0, 0.0);
}

Vector Sphere::normal(Point p)
{
	return p - this->center;
}

double Sphere::area()
{
	return 4.0*M_PI*this->radius*this->radius/3.0;
}

Photon Sphere::randomPhoton()
{
	double theta, phi, r;
	Point p;
	Ray ray;

	theta = random01()*2*M_PI;
	phi = acos(2*random01()-1);

	r = this->radius*sin(phi);
	p = Point(r*cos(theta), this->radius*cos(phi), r*sin(theta));

	/* TODO: just for white light, and not quite, see http://en.wikipedia.org/wiki/Planck%27s_law_of_black_body_radiation */
	ray = Ray(p, this->normal(p));
	return Photon(ray, random01() * (VISIBLE_U_WL - VISIBLE_L_WL) + VISIBLE_L_WL);
}

