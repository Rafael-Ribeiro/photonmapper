#include <math.h>

#include "Plane.hpp"

Plane::Plane(Material mat, Point p, Vector normal)
	: Primitive(mat), p(p), m_normal(normal)
{
}

Vector Plane::intersect(Ray r)
{
	/* TODO */
	return Vector(0,0,0);
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

