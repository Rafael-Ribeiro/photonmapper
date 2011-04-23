#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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
#define GREY     	0.9, 0.92, 0.92, 1.0
#define MAX_OBJS	10
#define MAX_POS		20
#define POS_INC		0.1

#define ANGLE_INC	0.01
#define TIMER		25

#define ESC_KEY		27

/* light constants */
#define CEIL_LIGHT				GL_LIGHT0
#define FLASH_LIGHT				GL_LIGHT1
#define FLASH_CUTOFF_ANGLE		((GLfloat)15.0)
#define FLASH_EXPONENT			((GLfloat)0.3)

/* globals */
GLint screenWidth = 800, screenHeight = 600;

GLfloat observerPos[] = { -3.0, 0.0, 0.0 };
GLfloat observerDir[] = { 1.0, 0.0, 0.0 };
GLfloat observerDirAngle = 0.0;
OBJECT objects[MAX_OBJS];

GLfloat ambientLightColorDay[4] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat ambientLightColorNight[4] = { 0.1, 0.1, 0.1, 1.0 };

GLfloat flashDir[] = { 1.0, 0.0, 0.0 };
GLfloat flashAngleHor = 0.0;
GLfloat flashAngleVer = 0.0;

int nobjects = 0;
bool color = false;
bool day = true;
bool ceilLightOn = false;
bool flashLightOn = false;

void drawObject(OBJECT* obj)
{
	GLfloat mat[4];

	glPushMatrix();
		glTranslatef(obj->pos[0], obj->pos[1], obj->pos[2]);
		glRotatef(obj->rotAngle, obj->rotAxis[0], obj->rotAxis[1], obj->rotAxis[2]);

		mat[0] = 0.24725; mat[1] = 0.1995; mat[2] = 0.0745; mat[3] = 1.0; /* red, green, blue and alpha ambient component */
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat);
		mat[0] = 0.75164; mat[1] = 0.60648; mat[2] = 0.22648; /* red, green and blue diffuse component */
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat);
		mat[0] = 0.628281; mat[1] = 0.555802; mat[2] = 0.366065; /* red, green and blue speculars */
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.4 * 128); /* shininess * 128 */

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
	glLightfv(FLASH_LIGHT, GL_SPOT_DIRECTION, flashDir);

	if (day)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorDay);
	else
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorNight);

	if (ceilLightOn)
		glEnable(CEIL_LIGHT);
	else
		glDisable(CEIL_LIGHT);

	if (flashLightOn)
		glEnable(FLASH_LIGHT);
	else
		glDisable(FLASH_LIGHT);
}

/* renderiza as views em 2D */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* main view */
	glViewport(0,0,screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, 4.0/3.0, 1.0, 100.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		observerPos[0], observerPos[1], observerPos[2],
		observerPos[0]+observerDir[0], observerPos[1]+observerDir[1], observerPos[2]+observerDir[2],
		0, 1, 0
	);

	draw();
	
	/* map at the bottom */
	glClear(GL_DEPTH_BUFFER_BIT);
	glScissor(0,0,screenWidth/3, screenHeight/3);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0,0,screenWidth/3, screenHeight/3);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-MAX_POS/2, MAX_POS,-MAX_POS/2, MAX_POS/2, 0, MAX_POS*2); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		0, MAX_POS, 0,	/* camera pos: above the map*/
		0, 0, 0, 		/* look at center */
		1, 0, 0 		/* camera top: front */
	);

	draw();

	glScissor(0, 0, screenWidth, screenHeight);
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
			ceilLightOn = !ceilLightOn;
			break;

		case 'f':
		case 'F':
			flashLightOn = !flashLightOn;				
			break;

		case 'a':
		case 'A':
			flashAngleHor -= ANGLE_INC*2*M_PI;
			if (flashAngleHor >= 2*M_PI)
				flashAngleHor -= 2*M_PI;
 
			flashDir[0] = cos(flashAngleHor);
			flashDir[2] = sin(flashAngleHor);
			break;

		case 'd':
		case 'D':
			flashAngleHor += ANGLE_INC*2*M_PI;
			if (flashAngleHor < 0)
				flashAngleHor += 2*M_PI;

			flashDir[0] = cos(flashAngleHor);
			flashDir[2] = sin(flashAngleHor);
			break;

		case 'w':
		case 'W':
			flashAngleVer += ANGLE_INC*2*M_PI;
			if (flashAngleVer >= 2*M_PI)
				flashAngleVer -= 2*M_PI;
		
			flashDir[1] = tan(flashAngleHor);
			break;

		case 's':
		case 'S':
			flashAngleVer -= ANGLE_INC*2*M_PI;
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
			observerDirAngle -= ANGLE_INC*2*M_PI;
			if (observerDirAngle < 0)
				observerDirAngle += 2*M_PI;

			observerDir[0] = cos(observerDirAngle);
			observerDir[2] = sin(observerDirAngle);
			break;

		case GLUT_KEY_RIGHT:
			observerDirAngle += ANGLE_INC*2*M_PI;
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

void timerCallback(int value) 
{
	int i;
	for (i = 0; i < nobjects; i++)
	{
		objects[i].rotAngle += ANGLE_INC*360.0;
		if (objects[i].rotAngle >= 360.0)
			objects[i].rotAngle -= 360.0;
	}

	glutPostRedisplay();
	glutTimerFunc(TIMER, timerCallback, 1);
}

/* Ligths setup */
void setupLighting()
{
	GLfloat flashLightColor[4] = {0.1, 0.1, 0.1, 1.0};
	GLfloat flashLightPos[4] = {-1.0, 0.0, 0.0, 1.0};
	GLfloat flashLightAttCon = 1.0;
	GLfloat flashLightAttLin = 0.05;
	GLfloat flashLightAttQua = 0.0;

	/* Ambient light */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightColorDay);

	/* Flashlight */
	glLightfv(FLASH_LIGHT, GL_POSITION, flashLightPos);
	glLightfv(FLASH_LIGHT, GL_AMBIENT, flashLightColor);
	glLightf (FLASH_LIGHT, GL_CONSTANT_ATTENUATION, flashLightAttCon);
	glLightf (FLASH_LIGHT, GL_LINEAR_ATTENUATION, flashLightAttLin);
	glLightf (FLASH_LIGHT, GL_QUADRATIC_ATTENUATION, flashLightAttQua);
	glLightf(FLASH_LIGHT, GL_SPOT_CUTOFF, FLASH_CUTOFF_ANGLE);
	glLightf(FLASH_LIGHT, GL_SPOT_EXPONENT, FLASH_EXPONENT);

	/* Ceil light */
}

/* inicializa o OpenGL e variáveis */
void init()
{
	glClearColor(GREY);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_LIGHTING);

	/* Object declaration */
	nobjects = 1;
	objects[0].type = t_teapot;
	objects[0].pos[0] = 1.0; objects[0].pos[1] = 0.0; objects[0].pos[2] = 0.0; 
	objects[0].rotAxis[0] = 0.0; objects[0].rotAxis[1] = 1.0; objects[0].rotAxis[2] = 0.0;
	objects[0].rotAngle = 0;
 
	setupLighting();
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
	
	glutTimerFunc(TIMER, timerCallback, 1);

	init();

	glutMainLoop();

	return 0;
}
