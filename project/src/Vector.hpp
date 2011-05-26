#ifndef VECTOR_HPP
#define VECTOR_HPP

struct Vector
{
	double x, y, z;
	
	Vector();
	Vector(double x, double y, double z);

	Vector operator + (const Vector& v) const;
	Vector operator - (const Vector& v) const;

	Vector operator * (double q) const;
	Vector operator / (double q) const;

	Vector cross(const Vector& v) const;
	double dot(const Vector& v) const;
	double angle(const Vector& v) const;

	double sqrd_norm() const;
	double norm() const;

	Vector normalize();
};

typedef Vector Point;
#endif
