#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glut.h>

#define M_PI  	3.141592653589793238 /* not ansi */

#define GREY 	0.90, 0.90, 0.90, 0.0
#define BLUE	0.0, 0.0, 1.0
#define ORANGE	1.0, 0.5, 0.0

#define ESC_KEY	27
#define TIMER	10

/* globals */
GLfloat screenWidth = 800, screenHeight = 600;
GLfloat speed = 0.005;
GLfloat transparency = 0.5;
GLfloat observerPos[] = { -10.0, 0.0, 0.0 };

GLfloat angle = 0.0;
GLfloat radius = 5.0;

/* cria os objectos no espaço */
void draw()
{
	if (angle >= M_PI)
	{
		glPushMatrix();
			glColor4f(BLUE, transparency);
			glutSolidSphere(1.0, 50.0, 50.0);
		glPopMatrix();

		glPushMatrix();
			glColor4f(ORANGE, transparency);
			glTranslatef(radius*sin(angle), 0, radius*cos(angle));
			glutSolidSphere(1.0, 50.0, 50.0);
		glPopMatrix();
	} else
	{
		glPushMatrix();
			glColor4f(ORANGE, transparency);
			glTranslatef(radius*sin(angle), 0, radius*cos(angle));
			glutSolidSphere(1.0, 50.0, 50.0);
		glPopMatrix();

		glPushMatrix();
			glColor4f(BLUE, transparency);
			glutSolidSphere(1.0, 50.0, 50.0);
		glPopMatrix();


	}
}

/* renderiza as views em 2D */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, screenWidth/screenHeight, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		observerPos[0], observerPos[1], observerPos[2],
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);

	draw();

	glutSwapBuffers();
}

/* keyboard handlers */
void keyboardASCIICallback(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 't':
		case 'T':
			transparency += 0.01;
			if (transparency > 1.0)
				transparency = 1.0;
			break;

		case 'r':
		case 'R':
			transparency -= 0.01;
			if (transparency < 0.0)
				transparency = 0.0;

			break;

		case 'a':
		case 'A':
			speed += 0.001;
			if (speed > 1.0)
				speed = 1.0;

			break;

		case 's':
		case 'S':
			speed -= 0.001;
			if (speed < 0.0)
				speed = 0.0;
			break;

		case ESC_KEY:
			exit(0);
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
	angle += speed*2*M_PI;
	if (angle >= 2*M_PI)
		angle -= 2*M_PI;

	glutPostRedisplay();
	glutTimerFunc(TIMER, timerCallback, 1);
}

/* inicializa o OpenGL e variáveis */
void init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glShadeModel(GL_SMOOTH);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight); 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("TP5 CG - jprafael@student.dei.uc.pt, jbaia@student.dei.uc.pt");
 	
	glutKeyboardFunc(keyboardASCIICallback);
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindowCallback);
	
	glutTimerFunc(TIMER, timerCallback, 1);
	
	glClearColor(GREY);
	init();

	glutMainLoop();

	return 0;
}
