#ifndef PLANE_HPP
#define PLANE_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

#include "jsonbox/inc/JsonBox.h"

struct Plane : public Primitive
{
	Point point;
	Vector m_normal;

	Plane();
	Plane(const Material& mat, const Point& point, const Vector& normal);

	static Plane *parse(const Material& mat, const JsonBox::Value &planeVal);
	bool intersect(const Ray& r, Point& p) const;
	Vector normal(const Point& p) const;
	double area() const;

	Photon randomPhoton() const;
};

#endif
