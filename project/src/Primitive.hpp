#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "Material.hpp"
#include "Vector.hpp"
#include "Ray.hpp"
#include "Photon.hpp"

struct Primitive
{
	Material mat;

	Primitive(const Material& mat);

	virtual bool intersect(const Ray& r, Point& p) const = 0;
	virtual Vector normal(const Point& p, double noise) const = 0;
	virtual double area() const = 0;

	virtual Photon randomPhoton() const = 0;
};

#endif
