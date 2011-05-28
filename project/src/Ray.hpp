#ifndef RAY_HPP
#define RAY_HPP

#include "Vector.hpp"
#include "Color.hpp"

 /*
  * FIXME:
  * Scene forward declaration;
  * remove this later if possible (since dependencies may change)
  */
struct Scene;
struct Primitive;

struct Ray
{
	Point origin;
	Vector direction;
	Primitive *inside;

	Ray();
	Ray(Point origin, Vector direction);

	Color getColor(const Scene& scene, int maxdepth, double nFrom) const;
};

#endif
