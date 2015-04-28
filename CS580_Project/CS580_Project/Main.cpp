#include "windows.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "objloader.h"

#undef main

float angle = 0.0;

objloader obj;

void loadFile(const char* fn, std::string& str)
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

unsigned int loadShader(std::string& source, unsigned int mode)
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

unsigned int vs, fs, program;

void initShader(const char* vname, const char* fname)
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

void clean()
{
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
}

static unsigned int loadTexture(const char* filename)
{
	unsigned int num;
	glGenTextures(1, &num);
	SDL_Surface* img = IMG_Load(filename);
	if (img == NULL)
	{
		std::cout << "img was not loaded" << std::endl;
		return -1;
	}
	SDL_PixelFormat form = { NULL, 32, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff, 0, 255 };
	SDL_Surface* img2 = SDL_ConvertSurface(img, &form, SDL_SWSURFACE);
	if (img2 == NULL)
	{
		std::cout << "img2 was not loaded" << std::endl;
		return -1;
	}
	glBindTexture(GL_TEXTURE_2D, num);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img2->pixels);
	SDL_FreeSurface(img);
	SDL_FreeSurface(img2);
	return num;
}

unsigned int myImg, myImg2, planet_earth;

void init()
{
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 800.0 / 600.0, 1, 1000);
	glTranslatef(0, 0, -8);
	glRotated(45, 1, 1, 1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	initShader("vertex.vs", "fragment.frag");
	myImg = loadTexture("brick.jpg");
	planet_earth = obj.load("sphere.obj");
}

void display()
{
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	angle += 3.0;
	if (angle >= 360)
		angle -= 360;

	glUniform3f(glGetUniformLocation(program, "lightPos"), 0.0, 0.0, 0.0);      //light position (is the same as the player position)

	glUniform3f(glGetUniformLocation(program, "mambient"), 0.2, 0.2, 0.2);      //setting the material property
	glUniform3f(glGetUniformLocation(program, "mdiffuse"), 0.6, 0.6, 0.6);
	//glUniform3f(glGetUniformLocation(program, "mspecular"), 1.0, 1.0, 1.0);
	glUniform3f(glGetUniformLocation(program, "mspecular"), 0.5, 0.5, 0.5);

	glUniform3f(glGetUniformLocation(program, "lambient"), 0.2, 0.2, 0.2);      //setting light property
	glUniform3f(glGetUniformLocation(program, "ldiffuse"), 0.6, 0.6, 0.6);
	//glUniform3f(glGetUniformLocation(program, "lspecular"), 1.0, 1.0, 1.0);
	glUniform3f(glGetUniformLocation(program, "lspecular"), 0.5, 0.5, 0.5);

	glUniform1f(glGetUniformLocation(program, "shininess"), 32.0);    //shininess

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myImg);
	glUniform1i(glGetUniformLocation(program, "img"), 0);

	glRotatef(angle, 0, 1, 0);
	glCallList(planet_earth);

	glPushMatrix();
	glTranslatef(-1, 0, -2);
	glCallList(planet_earth);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 0, -20);
	glCallList(planet_earth);
	glPopMatrix();
	/*
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(1, 0);              //gl_MultiTexCoord0
	glVertex3f(-1, 1, -4);    //gl_Vertex
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, -4);
	glTexCoord2f(0, 1);
	glVertex3f(1, -1, -4);
	glTexCoord2f(1, 1);
	glVertex3f(1, 1, -4);
	glEnd();
	*/
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
	Uint32 start;
	SDL_Event event;
	bool running = true;
	if (glewInit() != GLEW_OK) {
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
		if (1000.0 / 30 > SDL_GetTicks() - start)
			SDL_Delay(1000.0 / 30 - (SDL_GetTicks() - start));
	}
	clean();
	SDL_Quit();
}