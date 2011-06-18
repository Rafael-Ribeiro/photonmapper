#ifndef BOX_HPP
#define BOX_HPP

#include "Primitive.hpp"
#include "Vector.hpp"
#include "Quad.hpp"

/*
	Implements a planar convex quadrilateral defined as


	     +-----------+
	    /|          /|
	   / |         / |
	  /  |        /  |
	 /   |       /   |
	A'----------+    |
	|    |      |    |
	|    B'-----|----+
	|   /       |   /
	|  /        |  /
	| /         | /
	|/          |/
	S-----------C'

	A' = S + a
	B' = S + b
	C' = S + c
*/

struct Box : public Primitive
{
private:
	Quad quads[6];

public:
	Point source;
	Vector a, b, c;

	Box(const Material& mat, const Point& source, const Vector& a, const Vector& b, const Vector& c);

	bool intersect(const Ray& r, Point& p) const;
	Vector normal(const Point& p) const;
	double area() const;

	Photon randomPhoton() const;
};

#endif
