#include <iostream>

#include "Color.hpp"

void writePPM(Color* pixels, int width, int height, std::ostream& outputStream)
{
	/* PPM P6 file format */
	outputStream << "P6" << std::endl
		<< width << " " << height << std::endl
		<< 255 << std::endl;

	outputStream.write((char*)pixels, sizeof(Color) * width * height);
}
