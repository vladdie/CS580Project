#include "windows.h"
#include "objloader.h"
#include "main.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#undef main

/* GLOBALS */

unsigned int vs, fs, program;
objloader obj;
float angle = 0.0;

unsigned int planet_earth;
unsigned int planet_mars;
unsigned int planet_jupiter;
unsigned int planet_venus;

/* STATIC FUNCTIONS */

/*
* Shader loading functions
* The below shader loading code is provided by thecplusplusguy which is under LGPL
* http://www.youtube.com/user/thecplusplusguy
*/

static void loadFile(const char* fn, std::string& str)
{
	std::ifstream in(fn);
	if (!in.is_open())
	{
		std::cout << "The file " << fn << " cannot be opened\n";
		return;
	}
	char tmp[300];
	while (!in.eof())
	{
		in.getline(tmp, 300);
		str += tmp;
		str += '\n';
	}
}

static unsigned int loadShader(std::string& source, unsigned int mode)
{
	unsigned int id;
	id = glCreateShader(mode);

	const char* csource = source.c_str();

	glShaderSource(id, 1, &csource, NULL);
	glCompileShader(id);
	char error[1000];
	glGetShaderInfoLog(id, 1000, NULL, error);
	std::cout << "Compile status: \n" << error << std::endl;
	return id;
}

static void initShader(const char* vname, const char* fname)
{
	std::string source;
	loadFile(vname, source);
	vs = loadShader(source, GL_VERTEX_SHADER);
	source = "";
	loadFile(fname, source);
	fs = loadShader(source, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glUseProgram(program);
}

static void clean()
{
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
}

/*
* OpenGL init function
*/

void init()
{
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, WINDOW_WIDTH / WINDOW_HEIGHT, 1, 1000);

	glTranslatef(0, 0, -6);
	glRotated(30, 1, 1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	/* Load shaders */
	initShader("vertex.vs", "fragment.frag");

	/* Load obj files */
	planet_earth = obj.load("earth.obj");
	planet_mars = obj.load("mars.obj");
	planet_jupiter = obj.load("jupiter.obj");
	planet_venus = obj.load("venus.obj");
}

/*
* OpenGL display function
*/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

#ifdef ROTATE_PLANETS
	angle += 0.2;
	if (angle >= 360)
		angle -= 360;
#endif

	/* Earth - Front one */
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	glCallList(planet_earth);
	glPopMatrix();

	/* Earth - Back one for comparision */
	glPushMatrix();
	glTranslatef(-2, .5, -2);
	glRotatef(angle, 0, 1, 0);
	glCallList(planet_earth);
	glPopMatrix();

	/* Mars */
	glPushMatrix();
	glTranslatef(1, 1, -20);
	glRotatef(angle, 0, 1, 0);
	glCallList(planet_mars);
	glPopMatrix();

	/* Jupiter */
	glPushMatrix();
	glTranslatef(14, 4, -30);
	glScalef(8, 8, 8);
	glRotatef(angle, 0, 1, 0);
	glCallList(planet_jupiter);
	glPopMatrix();

	/* Venus */
	glPushMatrix();
	glTranslatef(.1, -.1, 3);
	glScalef(.5, .5, .5);
	glRotatef(angle, 0, 1, 0);
	glCallList(planet_venus);
	glPopMatrix();
}

/*
* Main function
*/

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL);
	Uint32 start;
	SDL_Event event;
	bool running = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW \n" << std::endl;
		return -1;
	}
	init();
	bool b = false;

	while (running)
	{
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				}
				break;
			}
		}
		display();
		SDL_GL_SwapBuffers();

		if (1000.0 / 30 > SDL_GetTicks() - start) {
			SDL_Delay(1000.0 / 30 - (SDL_GetTicks() - start));
		}
	}
	clean();
	SDL_Quit();
}