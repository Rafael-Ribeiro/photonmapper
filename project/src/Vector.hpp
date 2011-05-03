#ifndef VECTOR_HPP
#define VECTOR_HPP

struct Vector
{
	double x, y, z;
	
	Vector();
	Vector(double x, double y, double z);

	Vector operator + (Vector& v) const;
	Vector operator - (Vector& v) const;

	Vector operator * (double q) const;
	Vector operator / (double q) const;

	Vector cross(Vector& v) const;
	double dot(Vector& v) const;
	double angle(Vector& v) const;

	double norm() const;

	void normalize();
};

typedef Vector Point;
#endif
