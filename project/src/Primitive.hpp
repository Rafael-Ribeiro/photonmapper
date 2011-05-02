#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "Material.hpp"
#include "Vector.hpp"
#include "Ray.hpp"

struct Primitive
{
	Material mat;

	Primitive(Material& mat);

	virtual Vector intersect(Ray r) = 0;
	virtual Vector normal(Point p) = 0;
	virtual double area() = 0;
};

#endif
