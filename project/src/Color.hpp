#ifndef COLOR_HPP
#define COLOR_HPP

struct Color
{
	unsigned char r,g,b; 

	Color();
	Color(unsigned char r, unsigned char g, unsigned char b);

	Color operator + (Color other) const;
	Color operator * (double q) const;

} __attribute__((aligned(1)));

#endif
