#include "Sprite.h"
#include <cmath>
#include "Vertex.h"
#include <cstddef>


double xspeed = 0.0000004;   // ATTENTION : CES DEUX VARIABLES STOCKENT LA VITESSE DE CHAQUE SPRITE.
double yspeed = 0.000002;

//float vertexData[12];   // 6 because of the 6 vertices I need for a square (2triangles). 2 for the number of tringle.
Vertex vertexData[6];



Sprite::Sprite()
{
	//Always initialize your buffer IDs to 0
	_vboID = 0;
}


Sprite::~Sprite()
{
	//Always remember to delete your buffers when
	//you are done!
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}


void Sprite::init(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	
	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	//float vertexData[12];   // 6 because of the 6 vertices I need for a square (2triangles). 2 for the number of tringle.
	
	setVertexData();


	//Tell opengl to bind our vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	//Upload the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	//Unbind the buffer (optional)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Sprite::draw()
{
	//bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	//Tell opengl that we want to use the first
	//attribute array. We only need one array right
	//now since we are only using position.
	glEnableVertexAttribArray(0);

	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//This is the UV attribute pointer
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//Draw the 6 vertices to the screen
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Disable the vertex attrib array. This is not optional.
	glDisableVertexAttribArray(0);

	//Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}


void Sprite::moveThrough(double xdirection, double ydirection, double xspeedInit, double yspeedInit, double xacceleration, double yacceleration, bool random, double rainDropsRatio){  // raindDropsRatio pour le rapport entre la taille de cette goutte et la plus grosse goutte possible.
	// On va adpater les differentes vitesses à la taille de la goutte.
	xspeedInit *= rainDropsRatio;
	yspeedInit *= rainDropsRatio;
	xacceleration *= rainDropsRatio;
	yacceleration *= rainDropsRatio;

	if (_y == std::abs(_height - .001)) {   // Si on est à peu près en haut du machin.
		xspeed = xspeedInit;
		yspeed = yspeedInit;
	}


	// On vérifie qu'on ne dépasse pas du cadre sur x. Sinon on revient de l'autre bout.
	if (abs(xdirection*xspeed + _x) <= 1 + _width) {
		_x += xdirection*xspeed;
	}  
	
	else if (_x < 0) { 
		_x = 1- xdirection*xspeed;
		xspeed = xspeedInit;
		yspeed = yspeedInit;
	}
	else { 
		_x = xdirection*xspeed -1;
	}
	

	// De même pour y.  (Avec accélération).
	if (abs(ydirection*yspeed + _y) <= 1 + _height) {
		_y += ydirection*yspeed;
		yspeed += yacceleration;	 // Les accélérations.
		xspeed += xacceleration;	// Les accélérations.
	}    
	
	else if (_y < 0) {
		_y = 1 - ydirection*yspeed;
		if (random) {
			_x = (rand() % 200) / 100.0 - 1.0;
		}   // random la pluie  quand elle reapparait.
		yspeed = yspeedInit;
		xspeed = xspeedInit;
	}
	
	else { _y = ydirection*yspeed -1; }

	setVertexData();


	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	draw();

}

float Sprite::getYPos()
{
	return _y;
}

float Sprite::getXPos()
{
	return _x;
}

float Sprite::getWidth()
{
	return _width;
}

float Sprite::getHeight()
{
	return _height;
}


void Sprite::resize(float width, float height) {

	_width = width;
	_height = height;
	
	setVertexData();


	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	draw();

}


void Sprite::setVertexData() {
	// First Triangle
	vertexData[0].setPosition(_x + _width, _y + _height);
	//vertexData[0].setUV(1.0f, 1.0f);

	vertexData[1].setPosition(_x, _y + _height);
	//vertexData[1].setUV(0.0f, 1.0f);

	vertexData[2].setPosition(_x, _y);
	//vertexData[2].setUV(0.0f, 0.0f);   


	// Second one
	vertexData[3].setPosition(_x, _y);
	//vertexData[3].setUV(0.0f, 0.0f);

	vertexData[4].setPosition(_x + _width, _y);
	//vertexData[4].setUV(1.0f, 0.0f);

	vertexData[5].setPosition(_x + _width, _y + _height);
	//vertexData[5].setUV(1.0f, 1.0f);



	for (int i = 0; i < 6; i++) {
		vertexData[i].setColor(0, 0, 255, 255);
	}
	
	/*vertexData[4].color.r = 255;
	vertexData[4].color.g = 255;
	vertexData[4].color.b = 255;
	vertexData[4].color.a = 255;
	vertexData[2].color.r = 255;
	vertexData[2].color.g = 255;
	vertexData[2].color.b = 255;
	vertexData[2].color.a = 255;
	vertexData[5].color.r = 255;
	vertexData[5].color.g = 255;
	vertexData[5].color.b = 255;
	vertexData[5].color.a = 255;*/


}