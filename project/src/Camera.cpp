#include <cmath>
#include <iostream>

#include "Camera.hpp"
#include "Vector.hpp"

using namespace std;

Camera::Camera()
{
}

Camera::Camera(const Point& origin, const Vector& direction, const Vector& top, double fovy, int width, int height)
		: origin(origin), direction(direction), width(width), height(height)
{
	this->aspect = (1.0 * width) / height;
	this->halfHeight = height / 2.0;
	this->halfWidth = width / 2.0;

	this->top = top * tan(fovy);
	this->right = this->direction.cross(top) * tan(fovy) * (-this->aspect);
}

Ray Camera::rayTroughPixel(int x, int y)
{
	Ray ray;
	double dx, dy;

	dx = (x + 0.5 - this->halfWidth)/this->halfWidth;
	dy = (-y + 0.5 + this->halfHeight)/this->halfHeight;

	ray.origin = this->origin;
	ray.direction = (this->direction + this->top*dy + this->right*dx).normalized();
	ray.inside = NULL;

	return ray;
}

