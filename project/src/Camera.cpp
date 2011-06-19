#include <cmath>
#include <iostream>

#include "Camera.hpp"
#include "Vector.hpp"

using namespace std;

Camera::Camera()
{
}

Camera::Camera(const Point& origin, const Vector& direction, const Vector& top, double fovy, int width, int height, const string filename)
		: origin(origin), direction(direction), width(width), height(height), filename(filename)
{
	this->aspect = (1.0 * width) / height;
	this->halfHeight = height / 2.0;
	this->halfWidth = width / 2.0;

	this->top = top * tan(fovy);
	this->right = this->direction.cross(top) * tan(fovy) * (-this->aspect);
}

bool Camera::parse(const JsonBox::Value &cameraVal, Camera &camera)
{
	JsonBox::Object cameraObj;

	if (!cameraVal.isObject())
	{
		cerr << "Error: Camera must be an Object." << endl;
		return false;
	}

	cameraObj = cameraVal.getObject();

	if
	(
		!cameraObj["origin"].isArray() || !cameraObj["direction"].isArray() || !cameraObj["top"].isArray()
		||
		!cameraObj["origin"].getArray()[0].isNumber()	|| !cameraObj["origin"].getArray()[1].isNumber() || !cameraObj["origin"].getArray()[2].isNumber()
		||
		!cameraObj["direction"].getArray()[0].isNumber() || !cameraObj["direction"].getArray()[1].isNumber() || !cameraObj["direction"].getArray()[2].isNumber()
		||
		!cameraObj["top"].getArray()[0].isNumber() || !cameraObj["top"].getArray()[1].isNumber() || !cameraObj["top"].getArray()[2].isNumber()
	)
	{
		cerr << "Error: invalid Camera vectors (origin, direction and top)." << endl;
		return false;
	}

	if (!cameraObj["fovy"].isNumber() || !cameraObj["width"].isInteger() || !cameraObj["height"].isInteger() || !cameraObj["filename"].isString())
	{
		cerr << "Error: invalid Camera parameters (fovy, width, height and filename)." << endl;
		return false;
	}

	Point origin = Point(cameraObj["origin"].getArray()[0].getNumber(), cameraObj["origin"].getArray()[1].getNumber(), cameraObj["origin"].getArray()[2].getNumber());
	Vector direction = Point(cameraObj["direction"].getArray()[0].getNumber(), cameraObj["direction"].getArray()[1].getNumber(), cameraObj["direction"].getArray()[2].getNumber());
	Vector top = Point(cameraObj["top"].getArray()[0].getNumber(), cameraObj["top"].getArray()[1].getNumber(), cameraObj["top"].getArray()[2].getNumber());

	camera = Camera(origin, direction, top, cameraObj["fovy"].getNumber() * (2 * M_PI) / 360.0, cameraObj["width"].getInt(), cameraObj["height"].getInt(), cameraObj["filename"].getString());

	return true;
}

Ray Camera::rayThrough(double x, double y) const
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

