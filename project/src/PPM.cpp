#include <iostream>

#include "Color.hpp"

void writePPM(Color* pixels, int width, int height, std::ostream outputStream)
{
	/* PPM P6 file format */
	outputStream << "P6" << endl
		<< width << " " << height << endl
		<< 255 << endl;

	outputStream.write(pixels,sizeof(Color) * width * height);
}
