#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

#include "jsonbox/inc/JsonBox.h"

struct Sphere : public Primitive
{
private:
	double radius2;

public:
	Vector center;
	double radius;

	Sphere();
	Sphere(const Material& mat, const Vector& center, double radius);

	static Sphere *parse(const Material& mat, const JsonBox::Value &sphereVal);
	bool intersect(const Ray& r, Point& p) const;
	Vector normal(const Point& p) const;
	double area() const;

	Photon randomPhoton() const;
};

#endif
