#include <iostream>
#include <cmath>

#include "Plane.hpp"
#include "Engine.hpp"

#include "jsonbox/inc/JsonBox.h"

using namespace std;

Plane::Plane()
	: Primitive()
{
}

Plane::Plane(const Material& mat, const Point& p, const Vector& normal)
	: Primitive(mat), point(p), m_normal(normal)
{
}

Plane *Plane::parse(const Material& mat, const JsonBox::Value &planeVal)
{
	JsonBox::Object planeObj;

	if (!planeVal.isObject())
	{
		cerr << "Error: Plane must be an Object" << endl;
		return NULL;
	}

	planeObj = planeVal.getObject();

	if
	(
		!planeObj["point"].isArray() || !planeObj["normal"].isArray()
		||
		!planeObj["point"].getArray()[0].isNumber() || !planeObj["point"].getArray()[1].isNumber() || !planeObj["point"].getArray()[2].isNumber()
		||
		!planeObj["normal"].getArray()[0].isNumber() || !planeObj["normal"].getArray()[1].isNumber() || !planeObj["normal"].getArray()[2].isNumber()
	)
	{
		cerr << "Error: invalid Plane coords." << endl;
		return NULL;
	}

	Point point = Point(planeObj["point"].getArray()[0].getNumber(), planeObj["point"].getArray()[1].getNumber(), planeObj["point"].getArray()[2].getNumber());
	Vector normal = Vector(planeObj["normal"].getArray()[0].getNumber(), planeObj["normal"].getArray()[1].getNumber(), planeObj["normal"].getArray()[2].getNumber());

	return new Plane(mat, point, normal);
}

bool Plane::intersect(const Ray& r, Point& p) const
{
	/* http://en.wikipedia.org/wiki/Line-plane_intersection*/
	double a, b;
	Vector offset;

	if (r.inside == this)
		return false;

	a = this->m_normal.dot(r.direction);
	if (a == 0)	/* Ray and Plane are parallel -> no intersection or Line intersection FIXME */
		return false;

	b = this->m_normal.dot(this->point - r.origin);
	if (a*b < Engine::EPS) /* point is behind the ray's origin */
		return false;

	offset = r.direction * (b/a);
	p = r.origin + offset;

	return true;
}

Vector Plane::normal(const Point& p) const
{
	return this->m_normal;
}

double Plane::area() const
{
	return INFINITY;
}

Photon Plane::randomPhoton() const
{
	Ray r = Ray(Point(0,0,0), Vector(1,0,0));

	return Photon(r, this->mat.color);
}
