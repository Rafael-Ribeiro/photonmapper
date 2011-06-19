#include <cmath>
#include <iostream>

#include "Sphere.hpp"
#include "Vector.hpp"
#include "Photon.hpp"
#include "Engine.hpp"
#include "utils.hpp"

using namespace std;

Sphere::Sphere(const Material& mat, const Vector& center, double radius)
	: Primitive(mat), center(center), radius(radius)
{
	this->radius2 = radius*radius;
}

bool Sphere::intersect(const Ray& r, Point& p) const 
{
	double Tca, d2, Thc, Toff;
	Vector L, offset;

	L = this->center - r.origin;
	Tca = L.dot(r.direction);
	
	if (Tca < 0.0)
		return false;

	d2 = L.dot(L) - Tca*Tca; /* pitagora's theorem */
	if (d2 > this->radius2)
		return false;

	Thc = sqrt(this->radius2 - d2);

	Toff = Tca - Thc;
	if (Toff > Engine::EPS || (r.inside != this && Toff > -Engine::EPS))
		offset = r.direction * Toff;
	else
		offset = r.direction * (Tca + Thc);

	p = r.origin + offset;

	return true;
}

Vector Sphere::normal(const Point& p) const
{
	return (p - this->center)/this->radius;
}

double Sphere::area() const
{
	return 4.0*M_PI*this->radius*this->radius/3.0;
}

Photon Sphere::randomPhoton() const
{
	double theta, phi, r;
	Point p;
	Ray ray;

	theta = random01()*2*M_PI;
	phi = acos(2*random01()-1);

	r = this->radius*sin(phi);
	p = Point(this->center.x + r*cos(theta), this->center.y + this->radius*cos(phi), this->center.z + r*sin(theta));

	ray = Ray(p, this->normal(p).noise(this->mat.roughness, true));

	return Photon(ray, this->mat.color);
}

