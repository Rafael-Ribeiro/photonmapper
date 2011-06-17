#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

using namespace std;

struct Color
{
	int r,g,b; 

	Color();
	Color(int r, int g, int b);

	bool operator == (const Color& other) const;
	Color operator + (const Color& other) const;
	Color operator * (double q) const;
	Color operator * (Color c) const;
	Color operator / (double q) const;

	Color cap() const;

	friend ostream &operator << (ostream &stream, Color c);
};

#endif
