#ifndef VECTOR_HPP
#define VECTOR_HPP

struct Vector
{
	double x, y, z;
	
	Vector(double x, double y, double z);

	Vector operator + (Vector& v) const;
	Vector operator - (Vector& v) const;

	Vector operator * (double q) const;
	Vector operator / (double q) const;

	Vector cross(Vector& v) const;
	Vector dot(Vector& v) const;

	double norm() const;

	void normalize();
};

typedef Vector Point;
#endif
