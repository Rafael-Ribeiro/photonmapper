#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* structers */
typedef enum {t_cube, t_sphere, t_torus, t_icos, t_octa, t_teapot} object_type;
typedef struct
{
	object_type type;

	GLfloat pos[3];
	GLfloat rotAxis[3];
	GLfloat rotAngle;

	/* color, properties, etc */
} OBJECT;

/* constants */
#define M_PI		3.141592653589793238 /* not ansi */
#define GRAY     	0.9, 0.92, 0.92, 1.0
#define MAX_OBJS	10
#define MAX_POS		20
#define POS_INC		0.1
#define ANGLE_INC	0.03
#define FLASH_CUTOFF_ANGLE		((GLfloat)15.0)
#define FLASH_EXPONENT			((GLfloat)0.3)

#define ESC_KEY		27

/* globals */
GLint screenWidth = 800, screenHeight = 600;
GLfloat observerPos[] = { -1.0, 0.0, 0.0 };
GLfloat observerDir[] = { 1.0, 0.0, 0.0 };
GLfloat observerDirAngle = 0.0;
GLfloat flashDir[] = { 1.0, 0.0, 0.0 };
GLfloat flashAngleHor = 0.0;
GLfloat flashAngleVer = 0.0;
OBJECT objects[MAX_OBJS];

int nobjects = 0;
bool color = false;
bool day = true;
bool lightCeil = false;
bool lightFlash = false;

void drawObject(OBJECT* obj)
{
	glPushMatrix();
		glTranslatef(obj->pos[0], obj->pos[1], obj->pos[2]);
		glRotatef(obj->rotAngle, obj->rotAxis[0], obj->rotAxis[1], obj->rotAxis[2]);

		glColor3f(0.0,0.0,0.0);

		switch (obj->type)
		{
			case t_sphere:
				glutSolidSphere(1.0, 10, 10);
				break;

			case t_cube:
				glutSolidCube(1.0);
				break;

			case t_torus:
				glutSolidTorus(0.5, 1.0, 10, 5);
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

/* cria os objectos no espaço */
void draw()
{
	int i;
	for (i = 0; i < nobjects; i++)
		drawObject(&objects[i]); 

	/* TODO not yet sure whether the light position needs to be reset (possibly relative to the observer) */
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, flashDir);
}

/* renderiza as views em 2D */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glViewport(0,0,screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, 4.0/3.0, 1.0, 100.0);
	/* glOrtho(-xC,xC,-yC,yC,-zC,zC); */

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		observerPos[0], observerPos[1], observerPos[2],
		observerPos[0]+observerDir[0], observerPos[1]+observerDir[1], observerPos[2]+observerDir[2],
		0, 1, 0
	);

	draw();

	glutSwapBuffers();
}

/* keyboard handlers */
void keyboardASCIICallback(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'm':
		case 'M':
			color = !color;
			break;

		case 'n':
		case 'N':
			day = !day;
			break;

		case 't':
		case 'T':
			lightCeil = !lightCeil;
			break;

		case 'f':
		case 'F':
			lightFlash = !lightFlash;
			if (lightFlash)
				glEnable(GL_LIGHT0);
			else
				glDisable(GL_LIGHT0);
				
			break;

		case 'a':
		case 'A':
			flashAngleHor -= ANGLE_INC;
			if (flashAngleHor >= 2*M_PI)
				flashAngleHor -= 2*M_PI;
 
			flashDir[0] = cos(flashAngleHor);
			flashDir[2] = sin(flashAngleHor);
			break;

		case 'd':
		case 'D':
			flashAngleHor += ANGLE_INC;
			if (flashAngleHor < 0)
				flashAngleHor += 2*M_PI;

			flashDir[0] = cos(flashAngleHor);
			flashDir[2] = sin(flashAngleHor);
			break;

		case 'w':
		case 'W':
			flashAngleVer += ANGLE_INC;
			if (flashAngleVer >= 2*M_PI)
				flashAngleVer -= 2*M_PI;
		
			flashDir[1] = tan(flashAngleHor);
			break;

		case 's':
		case 'S':
			flashAngleVer -= ANGLE_INC;
			if (flashAngleHor < 0)
				flashAngleHor += 2*M_PI;

			flashDir[1] = tan(flashAngleHor);
			break;

		case ESC_KEY:
			exit(0);
			break;	
	}

	glutPostRedisplay();
}

void keyboardSpecialCallback(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			observerDirAngle -= ANGLE_INC;
			if (observerDirAngle < 0)
				observerDirAngle += 2*M_PI;

			observerDir[0] = cos(observerDirAngle);
			observerDir[2] = sin(observerDirAngle);
			break;

		case GLUT_KEY_RIGHT:
			observerDirAngle += ANGLE_INC;
			if (observerDirAngle >= 2*M_PI)
				observerDirAngle -= 2*M_PI;

			observerDir[0] = cos(observerDirAngle);
			observerDir[2] = sin(observerDirAngle);
			break;

		case GLUT_KEY_UP:
			observerPos[0] += cos(observerDirAngle)*POS_INC;
			observerPos[2] += sin(observerDirAngle)*POS_INC;
			break;

		case GLUT_KEY_DOWN:
			observerPos[0] -= cos(observerDirAngle)*POS_INC;
			observerPos[2] -= sin(observerDirAngle)*POS_INC;
			break;
			
		default:
			break;
	}

	glutPostRedisplay();
}

void resizeWindowCallback(GLsizei w, GLsizei h)
{
	screenWidth = w;
	screenHeight = h;
	glutPostRedisplay();
}

/* inicializa o OpenGL e variáveis */
void init()
{
	/* GLfloat flashPos[] = {observerPos[0],observerPos[1] /COMMENT + flash height relative to observer? COMMENT/,observerPos[2]}; */

	glClearColor(GRAY);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	/* Object declaration */
	nobjects = 1;
	objects[0].type = t_teapot;
	objects[0].pos[0] = 1.0; objects[0].pos[1] = 0.0; objects[0].pos[2] = 0.0; 
	objects[0].rotAxis[0] = 0.0; objects[0].rotAxis[0] = 1.0; objects[0].rotAxis[0] = 0.0;
	objects[0].rotAngle = 0;
	/* Object declaration */

	/* Flashlight */
	if (lightFlash)
		glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, observerPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, FLASH_CUTOFF_ANGLE);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, FLASH_EXPONENT);
	/* Flashlight */
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize(screenWidth, screenHeight); 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("TP4 CG - jprafael@student.dei.uc.pt, jbaia@student.dei.uc.pt");
 	
	glutKeyboardFunc(keyboardASCIICallback);
	glutSpecialFunc(keyboardSpecialCallback); 
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindowCallback);

	init();

	glutMainLoop();

	return 0;
}
