#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>

using namespace std;

struct Vector
{
	double x, y, z;
	
	Vector();
	Vector(double x, double y, double z);

	Vector operator - () const;

	Vector operator + (const Vector& v) const;
	Vector operator - (const Vector& v) const;
	Vector operator * (double q) const;
	Vector operator / (double q) const;

	Vector cross(const Vector& v) const;
	double dot(const Vector& v) const;
	double angle(const Vector& v) const;

	double sqrd_norm() const;
	double norm() const;

	Vector normalized() const;

	Vector noise(double noise) const;

	friend ostream &operator << (ostream &stream, Vector c);
};

typedef Vector Point;

#endif
