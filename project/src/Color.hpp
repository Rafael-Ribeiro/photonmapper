#ifndef COLOR_HPP
#define COLOR_HPP

struct Color
{
	int r,g,b; 

	Color();
	Color(int r, int g, int b);

	bool operator == (const Color other) const;
	Color operator + (const Color other) const;
	Color operator * (double q) const;
	Color operator / (double q) const;

	Color cap() const;
};

#endif
