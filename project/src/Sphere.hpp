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

	bool intersect(Ray r, Point& p) const;
	Vector normal(Point p) const;
	double area() const;

	Photon randomPhoton() const;
};

#endif
