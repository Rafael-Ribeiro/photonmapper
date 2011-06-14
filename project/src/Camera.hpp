#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vector.hpp"
#include "Ray.hpp"

struct Camera
{
private:
	Point origin;
	Vector direction, top, right;
	double aspect;
	double halfWidth, halfHeight;	

public:
	int width, height;

	Camera();
	Camera(const Point& origin, const Vector& direction, const Vector& top, double fovy, int width, int height);

	Ray rayThrough(double x, double y) const;
};

#endif
