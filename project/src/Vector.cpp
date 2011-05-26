#include <math.h>
#include "Vector.hpp"

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

double Vector::sqrd_norm() const
{
	return this->x*this->x + this->y*this->y + this->z*this->z;
}

/* functions */
double Vector::norm() const
{
	return sqrt(this->sqrd_norm());
}

Vector Vector::normalize()
{
	*this = *this/this->norm();

	return *this;
}

