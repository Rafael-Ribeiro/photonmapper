#include <math.h>

#include "Plane.hpp"

#include <iostream>
using namespace std;

Plane::Plane(Material mat, Point p, Vector normal)
	: Primitive(mat), point(p), m_normal(normal)
{
}

bool Plane::intersect(Ray r, Point& p) const
{
	/* http://en.wikipedia.org/wiki/Line-plane_intersection*/
	double a, b;
	Vector offset;

	a = this->m_normal.dot(r.direction);
	if (a == 0)	/* Ray and Plane are paralel -> no intersection or Line intersection FIXME */
		return false;

	b = this->m_normal.dot(this->point - r.origin);
	if (a*b < 0) /* point is behind the point */
		return false;

	offset = r.direction * (b/a);
	p = r.origin + offset;

	return true;
}

Vector Plane::normal(Point p) const
{
	return this->m_normal;
}

double Plane::area() const
{
	return INFINITY;
}

Photon Plane::randomPhoton() const
{
	Ray r = Ray(Point(0,0,0), Vector(1,0,0));
	return Photon(r, 0.0);
}

