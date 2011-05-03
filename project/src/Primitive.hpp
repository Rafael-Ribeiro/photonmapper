#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "Material.hpp"
#include "Vector.hpp"
#include "Ray.hpp"
#include "Photon.hpp"

struct Primitive
{
	Material mat;

	Primitive(Material& mat);

	virtual bool intersect(Ray r, Point& p) = 0;
	virtual Vector normal(Point p) = 0;
	virtual double area() = 0;

	virtual Photon randomPhoton() = 0;
};

#endif
