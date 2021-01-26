#pragma once
#include <GL/glew.h>
#include "Vertex.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height);

	void draw();

	void moveThrough(double xdirection, double ydirection, double xspeedInit, double yspeedInit, double xacceleration = 0.0, double yacceleration = 0.0, bool random = 0.0, double rainDropsRatio = 1);
	float getYPos();

	float getXPos();

	float getWidth();

	float getHeight();

	void setVertexData();

	void resize(float width, float height);

private :
	float _x;
	float _y;
	float _height;
	float _width;
	GLuint _vboID;   // GLuint = unsigned int of 32 bit.
};

