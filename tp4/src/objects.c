#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <GL/glut.h>

#include "structures.h"
#include "materials.h"
#include "externs.h"

OBJECT loadObject(char* file)
{
	OBJECT obj;
	char type[20];
	char mat[64];
	FILE* f = fopen(file, "r");

	fscanf(f, "%20s", type);

	if (strcmp(type, "teapot") == 0)
		obj.type = t_teapot;
	else if (strcmp(type, "cube") == 0)
		obj.type = t_cube;
	else if (strcmp(type, "sphere") == 0)
		obj.type = t_sphere;
	else if (strcmp(type, "torus") == 0)
		obj.type = t_torus;
	else if (strcmp(type, "icos") == 0)
		obj.type = t_icos;
	else if (strcmp(type, "octa") == 0)
		obj.type = t_octa;

	fscanf(f, "%f %f %f", &obj.pos[0], &obj.pos[1], &obj.pos[2]);
	fscanf(f, "%f %f %f", &obj.rotAxis[0], &obj.rotAxis[1], &obj.rotAxis[2]);
	fscanf(f, "%f", &obj.rotAngle);
	fscanf(f, "%64s", mat);
	obj.mat = loadMaterial(mat);

	fclose(f);

	return obj;
}

void drawObject(OBJECT* obj)
{
	glPushMatrix();
		glTranslatef(obj->pos[0], obj->pos[1], obj->pos[2]);
		glRotatef(obj->rotAngle, obj->rotAxis[0], obj->rotAxis[1], obj->rotAxis[2]);

		if (color)
			glColor3fv(obj->mat.ambient);
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obj->mat.ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obj->mat.diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obj->mat.specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, obj->mat.shininess * 128); /* shininess * 128 */
		}

		switch (obj->type)
		{
			case t_sphere:
				glutSolidSphere(1.0, 50, 50);
				break;

			case t_cube:
				glutSolidCube(1.0);
				break;

			case t_torus:
				glutSolidTorus(0.5, 1.0, 50, 50);
				break;

			case t_icos:
				glutSolidIcosahedron();
				break;

			case t_octa:
				glutSolidOctahedron();
				break;

			case t_teapot:
				glutSolidTeapot(1.0);
				break;
		}

	glPopMatrix();
}
