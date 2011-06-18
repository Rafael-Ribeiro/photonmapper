#include <iostream>
#include <cmath>

#include "Plane.hpp"
#include "Engine.hpp"

using namespace std;

Plane::Plane(const Material& mat, const Point& p, const Vector& normal)
	: Primitive(mat), point(p), m_normal(normal)
{
}

bool Plane::intersect(const Ray& r, Point& p) const
{
	/* http://en.wikipedia.org/wiki/Line-plane_intersection*/
	double a, b;
	Vector offset;

	if (r.inside == this)
		return false;

	a = this->m_normal.dot(r.direction);
	if (a == 0)	/* Ray and Plane are parallel -> no intersection or Line intersection FIXME */
		return false;

	b = this->m_normal.dot(this->point - r.origin);
	if (a*b < Engine::EPS) /* point is behind the ray's origin */
		return false;

	offset = r.direction * (b/a);
	p = r.origin + offset;

	return true;
}

Vector Plane::normal(const Point& p) const
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

	return Photon(r, this->mat.color);
}
