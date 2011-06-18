#include <cmath>
#include <iostream>

#include "Box.hpp"
#include "Quad.hpp"
#include "Vector.hpp"
#include "Photon.hpp"
#include "Engine.hpp"
#include "utils.hpp"

using namespace std;

Box::Box(const Material& mat, const Point& source, const Vector& a, const Vector& b, const Vector& c)
	: Primitive(mat), source(source), a(a), b(b), c(c)
{
	this->quads[0] = Quad(mat, source, source+b, source+c);	// floor
	this->quads[1] = Quad(mat, source+c, source+a+c, source); // front 
	this->quads[2] = Quad(mat, source+c, source+c+a, source+c+b); // right
	this->quads[3] = Quad(mat, source+c+b, source+b, source+a+b+c); // back
	this->quads[4] = Quad(mat, source+b, source, source+a+b); // left
	this->quads[5] = Quad(mat, source+a, source+a+b, source+a+c); // top
}

bool Box::intersect(const Ray& r, Point& p) const
{
	int i;
	Point temp;
	bool found = false;
	double mindist = -1.0, dist;

	for (i = 0; i < 6; i++)
	{
		if (this->quads[i].intersect(r, temp))
		{
			dist = (r.origin - temp).norm();

			if (!found || dist < mindist)
			{
				found = true;
				mindist = dist;
				p = temp;
			}
		}
	}

	return found;
}

Vector Box::normal(const Point& p) const
{
	int i;
	Vector normal;
	
	for (i = 0; i < 6; i++)
	{
		normal = this->quads[i].normal(p); 

		if (fabs(normal.dot(this->quads[i].a - p)) < Engine::EPS)
			return normal;
	}

	return Vector(-1,-1,-1);
}

double Box::area() const
{
	return this->a.norm() * this->b.norm() * this->c.norm();
}

Photon Box::randomPhoton() const
{
	// TODO

	Ray r = Ray(Point(0,0,0), Vector(1,0,0));

	return Photon(r, this->mat.color);
}
