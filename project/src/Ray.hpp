#ifndef RAY_HPP
#define RAY_HPP

#include "Vector.hpp"

struct Ray
{
	Point origin;
	Vector direction;

	Ray();
	Ray(Point origin, Vector direction);
};

#endif
