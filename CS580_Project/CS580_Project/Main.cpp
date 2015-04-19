#include "main.h"

#include <iostream>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/glut.h>

using namespace std;

float Rotation = 0.0f;

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(10, 3, 0, 0, 0, 0, 0, 1, 0);

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0, 0, -2);
	glRotatef(Rotation, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glutWireTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0, 0, 2);
	glRotatef(Rotation, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, .5);
	glRotatef(-Rotation, 0, 1, 0);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 2, 0);
	glutSolidCube(1.3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, -2.5, 0);
	glRotatef(-Rotation, 1, 1, 0); 
	glRotatef(90, 0, 1, 0);
	glutSolidSphere(1, 32, 32);
	glPopMatrix();

	glutSwapBuffers();
}

static void KeyboardCB(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case KEY_ESCAPE:
		exit(0);
		break;

	default:
		break;
	}
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	//glutIdleFunc(RenderSceneCB);
	glutKeyboardFunc(KeyboardCB);
}

void Initialize()
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspect = (GLfloat)WIDTH / HEIGHT;
	gluPerspective(FOV_ANGLE, aspect, Z_NEAR, Z_FAR);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(0.0, 0.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 10);
	glutCreateWindow("CS580 Project");

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	InitializeGlutCallbacks();
	Initialize();

	// vertex buffer

	// compile shaders

	glutMainLoop();
}