#include <math.h>

#include "Color.hpp"

Color::Color()
{
}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
	: r(r), g(g), b(b)
{
}

Color Color::operator * (double q) const
{
	unsigned int r = this->r*q;
	unsigned int g = this->g*q;
	unsigned int b = this->b*q;

	if (r > ((unsigned char)255)) r = ((unsigned char)255);
	if (g > ((unsigned char)255)) g = ((unsigned char)255);
	if (b > ((unsigned char)255)) b = ((unsigned char)255);

	return Color(r, g, b); 
}

Color Color::operator + (Color other) const
{
	unsigned int r = this->r + other.r;
	unsigned int g = this->g + other.g;
	unsigned int b = this->b + other.b;

	if (r > ((unsigned char)255)) r = ((unsigned char)255);
	if (g > ((unsigned char)255)) g = ((unsigned char)255);
	if (b > ((unsigned char)255)) b = ((unsigned char)255);

	return Color((unsigned char)r, (unsigned char)g, (unsigned char)b); 
}
