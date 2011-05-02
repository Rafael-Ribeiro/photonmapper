#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

struct Sphere : public Primitive
{
	Vector center;
	double radius;

	Sphere(Material mat, Vector center, double radius);

	Vector intersect(Ray r);
	Vector normal(Point p);
	double area();

	Photon randomPhoton();
};

#endif
