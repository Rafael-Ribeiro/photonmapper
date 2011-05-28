#include <cmath>
#include <iostream>

#include "Sphere.hpp"
#include "Vector.hpp"
#include "Photon.hpp"
#include "utils.hpp"

using namespace std;

Sphere::Sphere(Material mat, Vector center, double radius)
	: Primitive(mat), center(center), radius(radius)
{
	this->radius2 = radius*radius;
}

bool Sphere::intersect(Ray r, Point& p) const 
{
	double Tca, d2, Thc;
	Vector L, offset;

	L = this->center - r.origin;
	Tca = L.dot(r.direction);
	
	if (Tca < 0.0)
		return false;

	d2 = L.dot(L) - Tca*Tca; /* pitagora's theorem */
	if (d2 > this->radius2)
		return false;

	Thc = sqrt(this->radius2 - d2);
	offset = r.direction*(Tca - Thc);
	p = r.origin + offset;

	return true;
}

Vector Sphere::normal(Point p, double noise) const
{
	/*	TODO: random point in cone (axis = normal, radius = noise*M_PI/2) */ 

	return (p - this->center)/this->radius;
}

double Sphere::area() const
{
	return 4.0*M_PI*this->radius*this->radius/3.0;
}

Photon Sphere::randomPhoton() const
{
	double theta, phi, r;
	Point p;
	Ray ray;

	theta = random01()*2*M_PI;
	phi = acos(2*random01()-1);

	r = this->radius*sin(phi);
	p = Point(this->center.x + r*cos(theta), this->center.y + this->radius*cos(phi), this->center.z + r*sin(theta));

	/* TODO: just for white light, and not quite, see http://en.wikipedia.org/wiki/Planck%27s_law_of_black_body_radiation */
	ray = Ray(p, this->normal(p, 0));

	return Photon(ray, this->mat.color);
}

