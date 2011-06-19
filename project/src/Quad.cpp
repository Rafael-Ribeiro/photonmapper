#include <cmath>
#include <iostream>

#include "Quad.hpp"
#include "Vector.hpp"
#include "Photon.hpp"
#include "Engine.hpp"
#include "utils.hpp"

#include "jsonbox/inc/JsonBox.h"

using namespace std;

Quad::Quad()
	: Primitive()
{
}

Quad::Quad(const Material& mat, const Point& a, const Point& b, const Point& c)
	: Primitive(mat), a(a), b(b), c(c)
{
	this->u = b - a;
	this->v = c - a; 
	this->m_area = this->u.cross(this->v).norm();
	this->m_normal = this->u.cross(this->v).normalized();
}

Quad *Quad::parse(const Material& mat, const JsonBox::Value &quadVal)
{
	JsonBox::Object quadObj;

	if (!quadVal.isObject())
	{
		cerr << "Error: Quad must be an Object" << endl;
		return NULL;
	}

	quadObj = quadVal.getObject();

	if
	(
		!quadObj["pointA"].isArray() || !quadObj["pointB"].isArray() || !quadObj["pointC"].isArray()
		||
		!quadObj["pointA"].getArray()[0].isNumber()	|| !quadObj["pointA"].getArray()[1].isNumber() || !quadObj["pointA"].getArray()[2].isNumber()
		||
		!quadObj["pointB"].getArray()[0].isNumber() || !quadObj["pointB"].getArray()[1].isNumber() || !quadObj["pointB"].getArray()[2].isNumber()
		||
		!quadObj["pointC"].getArray()[0].isNumber() || !quadObj["pointC"].getArray()[1].isNumber() || !quadObj["pointC"].getArray()[2].isNumber()
	)
	{
		cerr << "Error: invalid Quad coords." << endl;
		return NULL;
	}

	Point a = Point(quadObj["pointA"].getArray()[0].getNumber(), quadObj["pointA"].getArray()[1].getNumber(), quadObj["pointA"].getArray()[2].getNumber());
	Point b = Point(quadObj["pointB"].getArray()[0].getNumber(), quadObj["pointB"].getArray()[1].getNumber(), quadObj["pointB"].getArray()[2].getNumber());
	Point c = Point(quadObj["pointC"].getArray()[0].getNumber(), quadObj["pointC"].getArray()[1].getNumber(), quadObj["pointC"].getArray()[2].getNumber());

	return new Quad(mat,a,b,c);
}

bool Quad::intersect(const Ray& r, Point& p) const 
{
	/* Plane ABC intersection */
	double a, b, dot;
	Vector offset;

	if (r.inside == this)
		return false;

	a = this->m_normal.dot(r.direction);
	if (a == 0)	/* Ray and Plane are parallel -> no intersection or Line intersection FIXME */
		return false;

	b = this->m_normal.dot(this->a - r.origin);
	if (a*b < Engine::EPS) /* point is behind the ray's origin */
		return false;

	offset = r.direction * (b/a);
	p = r.origin + offset;

	/* 
		o is the vector from A to P
		point P is inside only if the projections of o onto u (and v) are between 0 and |u| (and |v| respectivly)
		
		0 <= |o|*cos(theta) <= |u|
		0 <= |o|* (o . u)/(|o|*|u|) <= |u|
		0 <= (o . u) / |u| <= |u|
		0 <= o. u <= |u|^2  
	*/
	Vector o = p - this->a;
	dot = this->u.dot(o);
	if (dot < 0 || dot > this->u.sqrd_norm())
		return false;

	dot = this->v.dot(o);
	if (dot < 0 || dot > this->v.sqrd_norm())
		return false;

	return true;
}

Vector Quad::normal(const Point& p) const
{
	return this->m_normal;
}

double Quad::area() const
{
	return this->m_area;
}

Photon Quad::randomPhoton() const
{
	Point p;
	double alpha, beta;
	
	alpha = random01();
	beta = random01();
	p = this->a + this->u*alpha + this->v*beta;
	
	return Photon(Ray(p, this->m_normal.noise(this->mat.roughness, true)), this->mat.color);
}

