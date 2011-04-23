#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glut.h>

/* structers */
typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[3];
	GLfloat specular[3];
	GLfloat shininess;
} MATERIAL;
typedef enum {t_cube, t_sphere, t_torus, t_icos, t_octa, t_teapot} object_type;
typedef struct
{
	object_type type;

	GLfloat pos[3];
	GLfloat rotAxis[3]; 
	GLfloat rotAngle;
	
	MATERIAL mat;
} OBJECT;

/* constants */
#define M_PI		3.141592653589793238 /* not ansi */
#define GREY     	0.90, 0.90, 0.90, 1.0
#define BLACK     	0.0, 0.0, 0.0, 1.0
#define MAX_OBJS	10
#define MAX_POS		20
#define POS_INC		0.1

#define ANGLE_INC	0.01
#define TIMER		25

#define ESC_KEY		27

/* light constants */
#define CEIL_LIGHT					GL_LIGHT0

#define FLASHLIGHT_LIGHT			GL_LIGHT1
#define FLASHLIGHT_CUTOFF_ANGLE		((GLfloat)15.0)
#define FLASHLIGHT_EXPONENT			((GLfloat)0.3)
#define FLASHLIGHT_OFFSET_X			((GLfloat)0.0)
#define FLASHLIGHT_OFFSET_Y			((GLfloat)0.0)
#define FLASHLIGHT_OFFSET_Z			((GLfloat)0.0)


/* globals */
GLint screenWidth = 800, screenHeight = 600;

GLfloat observerPos[] = { -3.0, 0.0, 0.0 };
GLfloat observerDir[] = { 1.0, 0.0, 0.0 };
GLfloat observerDirAngle = 0.0;
OBJECT objects[MAX_OBJS];

GLfloat ambientLightColorDay[4] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat ambientLightColorNight[4] = { 0.1, 0.1, 0.1, 1.0 };
/* GLfloat ambientLightColorNight[4] = { 0.0, 0.0, 0.0, 1.0 }; //Completely dark */

GLfloat flashlightDir[] = { 1.0, 0.0, 0.0 };
GLfloat flashlightAngleHor = 0.0;
GLfloat flashlightAngleVer = 0.0;

int nobjects = 0;
bool color = false;
bool day = true;
bool ceilLightOn = false;
bool flashlightOn = true;

void drawObject(OBJECT* obj)
{
	glPushMatrix();
		glTranslatef(obj->pos[0], obj->pos[1], obj->pos[2]);
		glRotatef(obj->rotAngle, obj->rotAxis[0], obj->rotAxis[1], obj->rotAxis[2]);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obj->mat.ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obj->mat.diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obj->mat.specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, obj->mat.shininess * 128); /* shininess * 128 */

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
	GLfloat flashlightPos[4] = { observerPos[0] + FLASHLIGHT_OFFSET_X, observerPos[1] + FLASHLIGHT_OFFSET_Y, observerPos[2] + FLASHLIGHT_OFFSET_Z, 1.0 }; /* this allows a flashlight-observer "offset" */

	int i;

	if (day)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorDay);
	else
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLightColorNight);

	if (ceilLightOn)
		glEnable(CEIL_LIGHT);
	else
		glDisable(CEIL_LIGHT);

	if (flashlightOn)
	{
		glEnable(FLASHLIGHT_LIGHT);
		glLightfv(FLASHLIGHT_LIGHT, GL_POSITION, flashlightPos);
		glLightfv(FLASHLIGHT_LIGHT, GL_SPOT_DIRECTION, observerDir);
	}
	else
		glDisable(FLASHLIGHT_LIGHT);

	for (i = 0; i < nobjects; i++)
		drawObject(&objects[i]); 

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
			flashlightOn = !flashlightOn;				
			break;

		case 'a':
		case 'A':
			flashlightAngleHor -= ANGLE_INC*2*M_PI;
			if (flashlightAngleHor >= 2*M_PI)
				flashlightAngleHor -= 2*M_PI;
 
			flashlightDir[0] = cos(flashlightAngleHor);
			flashlightDir[2] = sin(flashlightAngleHor);
			break;

		case 'd':
		case 'D':
			flashlightAngleHor += ANGLE_INC*2*M_PI;
			if (flashlightAngleHor < 0)
				flashlightAngleHor += 2*M_PI;

			flashlightDir[0] = cos(flashlightAngleHor);
			flashlightDir[2] = sin(flashlightAngleHor);
			break;

		case 'w':
		case 'W':
			flashlightAngleVer += ANGLE_INC*2*M_PI;
			if (flashlightAngleVer >= 2*M_PI)
				flashlightAngleVer -= 2*M_PI;
		
			flashlightDir[1] = tan(flashlightAngleHor);
			break;

		case 's':
		case 'S':
			flashlightAngleVer -= ANGLE_INC*2*M_PI;
			if (flashlightAngleVer < 0)
				flashlightAngleVer += 2*M_PI;

			flashlightDir[1] = tan(flashlightAngleHor);
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
	GLfloat ceilPos[4] = { 0, 15, 0, 1.0 };
	GLfloat ceilAmbientColor[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ceilDiffuseColor[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ceilSpecularColor[4] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat ceilAttCon = 0.50;
	GLfloat ceilAttLin = 0.05;
	GLfloat ceilAttQua = 0.0;

	GLfloat flashlightPos[4] = { observerPos[0] + FLASHLIGHT_OFFSET_X, observerPos[1] + FLASHLIGHT_OFFSET_Y, observerPos[2] + FLASHLIGHT_OFFSET_Z, 1.0 }; /* this allows a flashlight-observer "offset" */

	GLfloat flashlightAmbientColor[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat flashlightDiffuseColor[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat flashlightSpecularColor[4] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat flashlightAttCon = 1.0;
	GLfloat flashlightAttLin = 0.05;
	GLfloat flashlightAttQua = 0.0;

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
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_LIGHTING);

	/* Object declaration */
	nobjects = 1;
	objects[0].type = t_teapot;
	objects[0].pos[0] = 0.0; objects[0].pos[1] = 0.0; objects[0].pos[2] = 0.0; 
	objects[0].rotAxis[0] = 0.0; objects[0].rotAxis[1] = 1.0; objects[0].rotAxis[2] = 0.0;
	objects[0].rotAngle = 0;

	objects[0].mat.ambient[0] = 0.24725; objects[0].mat.ambient[1] = 0.1995; objects[0].mat.ambient[2] = 0.0745; objects[0].mat.ambient[3] = 1.0; /* red, green, blue and alpha ambient component */		
	objects[0].mat.diffuse[0] = 0.75164; objects[0].mat.diffuse[0] = 0.60648; objects[0].mat.diffuse[0] = 0.22648; /* red, green and blue diffuse component */
	objects[0].mat.specular[0] = 0.628281; objects[0].mat.specular[1] = 0.555802; objects[0].mat.specular[2] = 0.366065; /* red, green and blue speculars */
	objects[0].mat.shininess = 0.4;
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
