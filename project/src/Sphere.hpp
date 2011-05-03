#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

struct Sphere : public Primitive
{
private:
	double radius2;

public:
	Vector center;
	double radius;

	Sphere(Material mat, Vector center, double radius);

	bool intersect(Ray r, Point& p);
	Vector normal(Point p);
	double area();

	Photon randomPhoton();
};

#endif
