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
	if (a == 0)	/* Ray and Plane are parallel -> no intersection or Line intersection FIXME */
		return false;

	b = this->m_normal.dot(this->point - r.origin);
	if (a*b <= 0) /* point is behind the ray's origin */
		return false;

	offset = r.direction * (b/a);
	p = r.origin + offset;

	return true;
}

Vector Plane::normal(Point p, double noise) const
{
	//double theta, phi;

	if (noise == 0)
		return this->m_normal;
	/*
		TODO: random point in cone (axis = normal, radius = noise*M_PI/2) 

		theta = random01()*2*M_PI;
		phi = acos(2*random01()-1);

		r = this->radius+*sin(phi);
		p = Point(this->center.x + r*cos(theta), this->center.y + this->radius*cos(phi), this->center.z + r*sin(theta));
	*/
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

