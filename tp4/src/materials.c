#include <stdio.h>

#include <GL/glut.h>

#include "structures.h"

MATERIAL loadMaterial(char* file)
{
	MATERIAL mat;
	FILE* f = fopen(file, "r");

	fscanf(f, "%f %f %f %f", &mat.ambient[0], &mat.ambient[1], &mat.ambient[2], &mat.ambient[3]);
	fscanf(f, "%f %f %f", &mat.diffuse[0], &mat.diffuse[1], &mat.diffuse[2]);
	fscanf(f, "%f %f %f", &mat.specular[0], &mat.specular[1], &mat.specular[2]);
	fscanf(f, "%f", &mat.shininess);

	fclose(f);

	return mat;
}
