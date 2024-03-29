#include <algorithm>
#include <iostream>

#include "Color.hpp"

using namespace std;

Color::Color()
{
}

Color::Color(int r, int g, int b)
	: r(r), g(g), b(b)
{
}

bool Color::operator == (const Color& other) const
{
	return this->r == other.r && this->g == other.g && this->b == other.b;
}

Color Color::operator + (const Color& other) const
{
	return Color(this->r + other.r, this->g + other.g, this->b + other.b); 
}

Color Color::operator * (double q) const
{
	return Color(this->r*q, this->g*q, this->b*q); 
}

Color Color::operator * (Color c) const
{
	return Color(this->r*c.r, this->g*c.g, this->b*c.b) / 255;
}

Color Color::operator / (double q) const
{
	return Color(this->r/q, this->g/q, this->b/q); 
}

Color Color::cap() const
{
	return Color(min(255, this->r), min(255, this->g), min(255, this->b)); 
}

ostream &operator << (ostream &stream, Color c)
{
	stream << "Color: (" << c.r << ", " << c.g << ", " << c.b << ")";
	return stream;
}
