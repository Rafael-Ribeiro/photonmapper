#include <iostream>
#include <stdlib.h>

#include "Color.hpp"

using namespace std;

void writePPM(Color* pixels, int width, int height, ostream& outputStream)
{
	int i;
	int size;

	/* PPM P6 file format */
	outputStream << "P6" << std::endl
		<< width << " " << height << std::endl
		<< 255 << std::endl;

	size = width * height;
	for (i = 0; i < size; i++)
	{
		outputStream.put((char) pixels[i].r);
		outputStream.put((char) pixels[i].g);
		outputStream.put((char) pixels[i].b);
	}
}

double random01()
{
	return rand()*1.0/RAND_MAX;	
}
