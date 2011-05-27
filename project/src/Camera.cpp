#include "Camera.hpp"
#include "Vector.hpp"

Camera::Camera()
{
}

Camera::Camera(Point origin, Vector direction, Vector top, double fovy)
		: origin(origin), direction(direction), top(top), fovy(fovy)
{
}
