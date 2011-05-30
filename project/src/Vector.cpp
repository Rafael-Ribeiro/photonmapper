#include "Vector.hpp"
#include "Engine.hpp"
#include "utils.hpp"

#include <cmath>
#include <iostream>

using namespace std;

Vector::Vector()
{
}

Vector::Vector(double x, double y, double z)
		: x(x), y(y), z(z)
{
}

/* operators */
Vector Vector::operator + (const Vector& v) const
{
	return Vector(this->x + v.x, this->y + v.y, this->z + v.z); 
}

Vector Vector::operator - () const
{
	return Vector(-this->x, -this->y, -this->z);
}

Vector Vector::operator - (const Vector& v) const
{
	return Vector(this->x - v.x, this->y - v.y, this->z - v.z); 
}

Vector Vector::operator * (double q) const
{
	return Vector(this->x*q, this->y*q, this->z*q); 
}

Vector Vector::operator / (double q) const
{
	return Vector(this->x/q, this->y/q, this->z/q); 
}

/* operator functions */
Vector Vector::cross(const Vector& v) const
{
	return Vector
	(
		this->y*v.z - this->z*v.y,
		this->z*v.x - this->x*v.z,
		this->x*v.y - this->y*v.x
	);
}

double Vector::dot(const Vector& v) const
{
	return this->x*v.x + this->y*v.y + this->z*v.z;
}

double Vector::angle(const Vector& v) const
{
	return acos(this->dot(v));
}

double Vector::sqrd_norm() const
{
	return this->x*this->x + this->y*this->y + this->z*this->z;
}

/* functions */
double Vector::norm() const
{
	return sqrt(this->sqrd_norm());
}

Vector Vector::normalized() const
{
	return *this/this->norm();
}

Vector Vector::noise(double noise) const
{
	//	TODO! if (noise < Engine::EPS)
		return Vector(*this);

	double radius, theta, a, b, norm;
	Vector u, v, z;

	/* point a, b, inside circle (biased noise towards the center, just how we like it) */
	norm = this->norm();
	radius = random01() * norm * noise;
	theta = random01() * (M_PI * 2);
	a = radius * cos(theta);
	b = radius * sin(theta); 
	
	/* u and v and this are ortonormal */
	z = *this/norm;
 	u = z.cross(Engine::top);
	v = z.cross(u);

	return z*(norm - radius) + u*a + v*b;
}
