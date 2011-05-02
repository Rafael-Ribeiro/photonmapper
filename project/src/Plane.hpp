#ifndef PLANE_HPP
#define PLANE_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

struct Plane : public Primitive
{
	Point p;
	Vector m_normal;

	Plane(Material mat, Point p, Vector normal);

	Vector intersect(Ray r);
	Vector normal(Point p);
	double area();
};

#endif
