#include <cmath>

#include "Sphere.hpp"
#include "Vector.hpp"
#include "Photon.hpp"

Sphere::Sphere(Material mat, Vector center, double radius)
	: Primitive(mat), center(center), radius(radius)
{
}

Vector Sphere::intersect(Ray r)
{
	// TODO 
	return Vector(0,0,0);
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

	theta = random01()*2*M_PI;
	phi = acos(2*random01()-1);

	r = this->radius*sin(phi);
	p = Point(r*cos(theta), this->radius*cos(phi), r*sin(theta));

	/* TODO: just for white light, and not quite, see http://en.wikipedia.org/wiki/Planck%27s_law_of_black_body_radiation */
	return Photon(Ray(p, this->normal(p), random01()*(750-390)+390);
}

