#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vector.hpp"

struct Camera
{
	Camera(Point origin, Vector direction, Vector top, double fovy);

	Point origin;
	Vector direction, top;
	double fovy;
};
#endif
