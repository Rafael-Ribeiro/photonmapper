#include <stdio.h>

#include <GL/glut.h>

#include "structures.h"

MATERIAL loadMaterial(char* file)
{
	MATERIAL mat;
	FILE* f = fopen(file, "r");
	
	mat.ambient[3] = 1.0;
	mat.color[3] = 1.0;

	fscanf(f, "%f %f %f", &mat.ambient[0], &mat.ambient[1], &mat.ambient[2]);
	fscanf(f, "%f %f %f", &mat.diffuse[0], &mat.diffuse[1], &mat.diffuse[2]);
	fscanf(f, "%f %f %f", &mat.specular[0], &mat.specular[1], &mat.specular[2]);
	fscanf(f, "%f %f %f", &mat.color[0], &mat.color[1], &mat.color[2]);
	fscanf(f, "%f", &mat.shininess);

	fclose(f);

	return mat;
}
