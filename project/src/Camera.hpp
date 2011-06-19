#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vector.hpp"
#include "Ray.hpp"

#include "jsonbox/inc/JsonBox.h"

struct Camera
{
private:
	Point origin;
	Vector direction, top, right;
	double aspect;
	double halfWidth, halfHeight;	

public:
	int width, height;
	string filename;

	Camera();
	Camera(const Point& origin, const Vector& direction, const Vector& top, double fovy, int width, int height, const string filename);

	static bool parse(const JsonBox::Value &cameraVal, Camera &camera);
	Ray rayThrough(double x, double y) const;
};

#endif
