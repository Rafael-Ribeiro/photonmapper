#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glut.h>

#define M_PI  	3.141592653589793238 /* not ansi */

#define BLACK     0.0, 0.0, 0.0
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

GLfloat black[4] = {BLACK, 0.0},
		blue[4] = {BLUE, 0.0},
		orange[4] = {ORANGE, 0.0};

/* cria os objectos no espaço */
void draw()
{

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);

	/* draw "transparency" */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glPushMatrix();
		glColor4f(BLUE, transparency);
		glutSolidSphere(1.0, 50.0, 50.0);
	glPopMatrix();

	glPushMatrix();
		glColor4f(ORANGE, transparency);
		glTranslatef(radius*sin(angle), 0, radius*cos(angle));
		glutSolidSphere(1.0, 50.0, 50.0);
	glPopMatrix();

	/* draw speculars */
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);	
	glDepthFunc(GL_EQUAL);

	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5 * 128); /* shininess * 128 */

		glutSolidSphere(1.0, 50.0, 50.0);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, orange);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5 * 128); /* shininess * 128 */

		glutSolidSphere(1.0, 50.0, 50.0);
	glPopMatrix();
}

/* renderiza as views em 2D */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	glutPostRedisplay();
	glutTimerFunc(TIMER, timerCallback, 1);
}

/* inicializa o OpenGL e variáveis */
void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glShadeModel(GL_SMOOTH);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
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
