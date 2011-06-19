#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <map>

#include "Material.hpp"
#include "Vector.hpp"
#include "Ray.hpp"
#include "Photon.hpp"

#include "jsonbox/inc/JsonBox.h"

struct Primitive
{
	Material mat;

	Primitive();
	Primitive(const Material& mat);

	static Primitive *parse(const JsonBox::Value &primitiveVal, map<string,Material>& materials);
	virtual bool intersect(const Ray& r, Point& p) const = 0;
	virtual Vector normal(const Point& p) const = 0;
	virtual double area() const = 0;

	virtual Photon randomPhoton() const = 0;
};

#endif
