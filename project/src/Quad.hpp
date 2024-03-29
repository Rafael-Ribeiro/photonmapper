#ifndef QUAD_HPP
#define QUAD_HPP

#include "Primitive.hpp"
#include "Vector.hpp"

#include "jsonbox/inc/JsonBox.h"

/*
	Implements a planar convex quadrilateral defined as

	     B-----------+
	    /           /
	   /           /
	  /           /
	 /           /
	A-----------C
*/
struct Quad : public Primitive
{
private:
	Vector u, v, m_normal;
	double m_area;

public:
	Point a, b, c;

	Quad();
	Quad(const Material& mat, const Point& a, const Point& b, const Point& c);

	static Quad *parse(const Material& mat, const JsonBox::Value &quadVal);
	bool intersect(const Ray& r, Point& p) const;
	Vector normal(const Point& p) const;
	double area() const;

	Photon randomPhoton() const;
};

#endif
