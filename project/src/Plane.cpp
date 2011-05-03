#include <math.h>

#include "Plane.hpp"

Plane::Plane(Material mat, Point p, Vector normal)
	: Primitive(mat), point(p), m_normal(normal)
{
}

bool Plane::intersect(Ray r, Point& p)
{
	/* http://en.wikipedia.org/wiki/Line-plane_intersection*/
	double a, b;
	Vector offset;

	a = r.direction.dot(this->m_normal);
	if (a == 0)	/* Ray and Plane are paralel -> no intersection or Line intersection FIXME */
		return false;

	b = (this->point - r.origin).dot(this->m_normal);
	if (b < 0) /* point is behind the ray */
		return false;

	offset = r.direction * (b/a);
	p = r.origin + offset;

	return true;
}

Vector Plane::normal(Point p)
{
	return this->m_normal;
}

double Plane::area()
{
	return INFINITY;
}

Photon Plane::randomPhoton()
{
	Ray r = Ray(Point(0,0,0), Vector(1,0,0));
	return Photon(r, 0.0);
}

