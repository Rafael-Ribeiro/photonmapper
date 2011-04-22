#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* constants */
#define GRAY		0.9, 0.92, 0.29, 1.0
#define ESC_KEY		27

/* globals */
GLint screenWidth= 800, screenHeight=600;
GLfloat observerPos[] = { 0.0, 0.0, 0.0 };

/* cria os objectos no espaço */
void draw()
{
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
	gluLookAt(observerPos[0], observerPos[1], observerPos[2], 0, 0, 0, 0, 1, 0); /* TODO */
	
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
			break;

		case 'n':
		case 'N':
			break;

		case 't':
		case 'T':
			break;

		case 'f':
		case 'F':
			break;

		case 'a':
		case 'A':
			break;

		case 'd':
		case 'D':
			break;

		case 'w':
		case 'W':
			break;

		case 's':
		case 'S':
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
			break;

		case GLUT_KEY_RIGHT:
			break;

		case GLUT_KEY_UP:
			break;

		case GLUT_KEY_DOWN:
			break;
			
		default:
			break;
	}

	glutPostRedisplay();
}

void resizeWindowCallback(GLsizei w, GLsizei h)
{
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize(screenWidth, screenHeight); 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("TP4 CG - jprafael@student.dei.uc.pt, jbaia@student.dei.uc.pt");
 
	glClearColor(GRAY);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	
	glutKeyboardFunc(keyboardASCIICallback);
	glutSpecialFunc(keyboardSpecialCallback); 
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindowCallback);
	
	glutMainLoop();

	return 0;
}
