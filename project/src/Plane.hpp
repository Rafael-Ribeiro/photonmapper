#ifndef PLANE_HPP
#define PLANE_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

struct Plane : public Primitive
{
	Point point;
	Vector m_normal;

	Plane(Material mat, Point point, Vector normal);

	bool intersect(Ray r, Point& p);
	Vector normal(Point p);
	double area();

	Photon randomPhoton();
};

#endif
