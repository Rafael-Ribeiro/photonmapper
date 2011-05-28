#include <math.h>

#include "Color.hpp"

Color::Color()
{
}

Color::Color(int r, int g, int b)
	: r(r), g(g), b(b)
{
}

Color Color::operator * (double q) const
{
	return Color(r*q, g*q, b*q); 
}

Color Color::operator + (Color other) const
{
	return Color(this->r + other.r, this->g + other.g, this->b + other.b); 
}

Color Color::cap() const
{
	return Color(min(255, this->r), min(255, this->g), min(255, this->b)); 
}
