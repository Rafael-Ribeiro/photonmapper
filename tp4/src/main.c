#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <GL/glut.h>

#include "structures.h"
#include "constants.h"
#include "materials.h"
#include "objects.h"
#include "polygons.h"

/* globals */
GLfloat screenWidth = 800, screenHeight = 600;

GLfloat observerPos[] = { -3.0, 0.0, 0.0 };
GLfloat observerDir[] = { 1.0, 0.0, 0.0 };
GLfloat observerDirAngle = 0.0;
OBJECT objects[MAX_OBJS];
POLY polys[MAX_POLYS];

GLfloat ambientLightColorDay[4] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat ambientLightColorNight[4] = { 0.1, 0.1, 0.1, 1.0 };

/*ceiling */
GLfloat ceilPos[4] = { 0, 15, 0, 1.0 };
GLfloat ceilAmbientColor[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat ceilDiffuseColor[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat ceilSpecularColor[4] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat ceilAttCon = 0.50;
GLfloat ceilAttLin = 0.05;
GLfloat ceilAttQua = 0.0;

/* flashlight */
GLfloat flashlightDir[] = { 1.0, 0.0, 0.0 };
GLfloat flashlightAngleHor = 0.0;
GLfloat flashlightAngleVer = 0.0;

GLfloat flashlightAmbientColor[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat flashlightDiffuseColor[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat flashlightSpecularColor[4] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat flashlightAttCon = 1.0;
GLfloat flashlightAttLin = 0.05;
GLfloat flashlightAttQua = 0.0;

int nobjects = 0;
int npolys = 0;
bool color = false;
bool day = true;
bool ceilLightOn = false;
bool flashlightOn = false;

void updateFlashlight()
{
	float r;

	r = cos(flashlightAngleVer);

	flashlightDir[0] = r*cos(flashlightAngleHor+observerDirAngle);
	flashlightDir[1] = sin(flashlightAngleVer);
	flashlightDir[2] = r*sin(flashlightAngleHor+observerDirAngle);
}

void draw_text(void* font, int x, int y, char* format, ...)
{
	char str[128];
	int i, n;

	va_list argp;

	va_start(argp, format);
	vsprintf(str, format, argp);
	va_end(argp);

	n = strlen(str);

	glRasterPos2i(x, y);
	for (i = 0; i < n; i++)
		glutBitmapCharacter(font, str[i]);
}

/* cria os objectos no espaço */
void draw()
{
	GLfloat flashlightPos[4] = { observerPos[0] + FLASHLIGHT_OFFSET_X, observerPos[1] + FLASHLIGHT_OFFSET_Y, observerPos[2] + FLASHLIGHT_OFFSET_Z, 1.0 }; /* this allows a flashlight-observer "offset" */

	int i;

	if (color)
	{
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}

	if (day)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorDay);
	else
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorNight);

	if (!day && ceilLightOn)
		glEnable(CEIL_LIGHT);
	else
		glDisable(CEIL_LIGHT);

	if (!day && flashlightOn)
	{
		glEnable(FLASHLIGHT_LIGHT);
		glLightfv(FLASHLIGHT_LIGHT, GL_POSITION, flashlightPos);
		glLightfv(FLASHLIGHT_LIGHT, GL_SPOT_DIRECTION, flashlightDir);
	} else
		glDisable(FLASHLIGHT_LIGHT);

	for (i = 0; i < nobjects; i++)
		drawObject(&objects[i]); 

	for (i = 0; i < npolys; i++)
		drawPoly(&polys[i]);

	if (color)
		glDisable(GL_COLOR_MATERIAL);
}

/* renderiza as views em 2D */
void display()
{
	if (day)
		glClearColor(LIGHT_BLUE);
	else
		glClearColor(BLACK);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* main view */
	glViewport(0,0,screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, screenWidth/screenHeight, 1.0, 100.0);
	
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
	glScissor(0,0,screenHeight/3, screenHeight/3);
	glClearColor(GREY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0,0, screenHeight/3, screenHeight/3);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-MAX_POS, MAX_POS,-MAX_POS, MAX_POS, 0, MAX_POS*2); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		observerPos[0], MAX_POS, observerPos[2],		/* camera pos: above the map*/
		observerPos[0], observerPos[1], observerPos[2], /* look at observer */
		observerDir[0], observerDir[1], observerDir[2] 	/* camera top: observer dir*/
	);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 0.0, 0.0); 
	glBegin(GL_LINES);
		glVertex3d(observerPos[0], 0.0, observerPos[2]);
		glVertex3d(observerPos[0] + observerDir[0]*2, 0.0, observerPos[2]+observerDir[2]*2);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);	

	draw();
	
	glScissor(0, 0, screenWidth, screenHeight);

	draw_text(GLUT_BITMAP_9_BY_15, 0, 0, "phase of the day: %s", day ? "day" : "night"); 
	draw_text(GLUT_BITMAP_9_BY_15, 20, 10, "ceiling: %s", ceilLightOn ? "on" : "off"); 
	draw_text(GLUT_BITMAP_9_BY_15, 30, 10, "flashlight: %s", flashlightOn ? "on" : "off"); 
	draw_text(GLUT_BITMAP_9_BY_15, 40, 10, "color material: %s", color ? "on" : "off"); 

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
			if (day)
			{
				flashlightAngleHor = flashlightAngleVer = 0.0;
				updateFlashlight();
			}

			break;
		case 't':
		case 'T':
			ceilLightOn = !ceilLightOn;
			break;

		case 'f':
		case 'F':
			flashlightOn = !flashlightOn;				
			break;

		case 'a':
		case 'A':
			flashlightAngleHor -= ANGLE_INC*2*M_PI;
			if (flashlightAngleHor <= -M_PI/2)
				flashlightAngleHor = -M_PI/2;
 
			updateFlashlight();

			break;

		case 'd':
		case 'D':
			flashlightAngleHor += ANGLE_INC*2*M_PI;
			if (flashlightAngleHor >= M_PI/2)
				flashlightAngleHor = M_PI/2;

			updateFlashlight();
			break;

		case 'w':
		case 'W':
			flashlightAngleVer += ANGLE_INC*2*M_PI;
			if (flashlightAngleVer >= M_PI/2)
				flashlightAngleVer = M_PI/2;
		
			updateFlashlight();
			break;

		case 's':
		case 'S':
			flashlightAngleVer -= ANGLE_INC*2*M_PI;
			if (flashlightAngleVer <= -M_PI/2)
				flashlightAngleVer = -M_PI/2;

			updateFlashlight();
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

	updateFlashlight();
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
		objects[i].rotAngle += ANGLE_INC/10.0*360.0;
		if (objects[i].rotAngle >= 360.0)
			objects[i].rotAngle -= 360.0;
	}

	glutPostRedisplay();
	glutTimerFunc(TIMER, timerCallback, 1);
}

/* Ligths setup */
void setupLighting()
{
	GLfloat flashlightPos[4] = { observerPos[0] + FLASHLIGHT_OFFSET_X, observerPos[1] + FLASHLIGHT_OFFSET_Y, observerPos[2] + FLASHLIGHT_OFFSET_Z, 1.0 }; /* this allows a flashlight-observer "offset" */

	/* Ambient light */
	if (day)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorDay);
	else
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorNight);

	/* Flashlight */
	if (flashlightOn)
		glEnable(FLASHLIGHT_LIGHT);

	glLightfv(FLASHLIGHT_LIGHT, GL_POSITION, flashlightPos);

	glLightfv(FLASHLIGHT_LIGHT, GL_SPOT_DIRECTION, flashlightDir);
	glLightf(FLASHLIGHT_LIGHT, GL_SPOT_CUTOFF, FLASHLIGHT_CUTOFF_ANGLE);
	glLightf(FLASHLIGHT_LIGHT, GL_SPOT_EXPONENT, FLASHLIGHT_EXPONENT);

	glLightfv(FLASHLIGHT_LIGHT, GL_AMBIENT, flashlightAmbientColor);
	glLightfv(FLASHLIGHT_LIGHT, GL_DIFFUSE, flashlightDiffuseColor);
	glLightfv(FLASHLIGHT_LIGHT, GL_SPECULAR, flashlightSpecularColor);

	glLightf(FLASHLIGHT_LIGHT, GL_CONSTANT_ATTENUATION, flashlightAttCon);
	glLightf(FLASHLIGHT_LIGHT, GL_LINEAR_ATTENUATION, flashlightAttLin);
	glLightf(FLASHLIGHT_LIGHT, GL_QUADRATIC_ATTENUATION, flashlightAttQua);

	/* Ceil light */
	if (ceilLightOn)
		glEnable(CEIL_LIGHT);

	glLightfv(CEIL_LIGHT, GL_POSITION, ceilPos);

	glLightfv(CEIL_LIGHT, GL_AMBIENT, ceilAmbientColor);
	glLightfv(CEIL_LIGHT, GL_DIFFUSE, ceilDiffuseColor);
	glLightfv(CEIL_LIGHT, GL_SPECULAR, ceilSpecularColor);

	glLightf(CEIL_LIGHT, GL_CONSTANT_ATTENUATION, ceilAttCon);
	glLightf(CEIL_LIGHT, GL_LINEAR_ATTENUATION, ceilAttLin);
	glLightf(CEIL_LIGHT, GL_QUADRATIC_ATTENUATION, ceilAttQua);
}

/* inicializa o OpenGL e variáveis */
void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);
	
	/* Object declaration */
	nobjects = 5;
	objects[0] = loadObject("objects/brass_teapot");
	objects[1] = loadObject("objects/chrome_sphere");
	objects[2] = loadObject("objects/yellow_rubber_torus");
	objects[3] = loadObject("objects/white_plastic_cube");
	objects[4] = loadObject("objects/emerald_icos");

	npolys = 1;
	polys[0] = loadPolygon("polygons/poly1");

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
