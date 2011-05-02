#include <math.h>

#include "Sphere.hpp"

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
