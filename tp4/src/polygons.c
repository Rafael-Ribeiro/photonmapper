#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <GL/glut.h>

#include "structures.h"
#include "materials.h"
#include "externs.h"

POLY loadPolygon(char* file)
{
	POLY pol;
	char mat[64];
	int i;
	GLfloat vec[2][3];

	FILE* f = fopen(file, "r");

	for (i = 0; i < 4; i++)
		fscanf(f, "%f %f %f", &pol.vertex[i][0], &pol.vertex[i][1], &pol.vertex[i][2]);

	fscanf(f, "%f %f %f", &pol.center[0], &pol.center[1], &pol.center[2]);
	fscanf(f, "%f %f %f", &pol.pos[0], &pol.pos[1], &pol.pos[2]);

	vec[0][0] = pol.vertex[1][0] - pol.vertex[0][0];
	vec[0][1] = pol.vertex[1][1] - pol.vertex[0][1];
	vec[0][2] = pol.vertex[1][2] - pol.vertex[0][2];
	vec[1][0] = pol.vertex[3][0] - pol.vertex[0][0];
	vec[1][1] = pol.vertex[3][1] - pol.vertex[0][1];
	vec[1][2] = pol.vertex[3][2] - pol.vertex[0][2];
	
	pol.normal[0] = vec[0][1]*vec[1][2] - vec[0][2]*vec[1][1];
	pol.normal[1] = vec[0][2]*vec[1][0] - vec[0][0]*vec[1][2];
	pol.normal[2] = vec[0][0]*vec[1][1] - vec[0][1]*vec[1][0];
	
	fscanf(f, "%f", &pol.rotAngle);
	fscanf(f, "%64s", mat);
	pol.mat = loadMaterial(mat);

	fclose(f);

	return pol;
}

void drawPoly(POLY* poly)
{
	glPushMatrix();
		glTranslatef(poly->pos[0], poly->pos[1], poly->pos[2]);
		glRotatef(poly->rotAngle, 0.0, 1.0, 0.0);
		glTranslatef(-poly->center[0], -poly->center[1], -poly->center[2]);

/*
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3fv(poly->normal);
		glEnd();
*/
		if (color)
			glColor3fv(poly->mat.color);
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, poly->mat.ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, poly->mat.diffuse);

			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, poly->mat.specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, poly->mat.shininess * 128); /* shininess * 128 */
		}

		glBegin(GL_QUADS);
			glNormal3fv(poly->normal);
			glVertex3fv(poly->vertex[0]);
			glVertex3fv(poly->vertex[1]);
			glVertex3fv(poly->vertex[2]);
			glVertex3fv(poly->vertex[3]);
		glEnd();
	glPopMatrix();
}
