#ifndef PLANE_HPP
#define PLANE_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

struct Plane : public Primitive
{
	Point point;
	Vector m_normal;

	Plane(const Material& mat, const Point& point, const Vector& normal);

	bool intersect(const Ray& r, Point& p) const;
	Vector normal(const Point& p, double noise) const;
	double area() const;

	Photon randomPhoton() const;
};

#endif
