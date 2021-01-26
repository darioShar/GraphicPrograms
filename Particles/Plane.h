#pragma once
#include <SFML\Graphics.hpp>
#include <thread>
#include "Particle.h"
#include "BlackHole.h"
#include <iostream>


#define MAX_NUM_PARTICLES 1000000
#define PLANE_WIDTH 1920
#define PLANE_HEIGHT 1080
/*
#define INITIAL_PARTICLE_WIDTH 500
#define INITIAL_PARTICLE_HEIGHT 500
#define NUM_PARTICLES INITIAL_PARTICLE_HEIGHT*INITIAL_PARTICLE_WIDTH
*/

using namespace std;
using namespace sf;


class Plane : public RenderWindow
{
public:
	Plane();
	~Plane();

	void clearPlane();
	void drawPlane();
	void updateParticlesPosition(float dt, bool LMB, Vector2f mousepos);
	void centerParticlesPosition();

	/*
	void addBlackHole(Vector2f position = Vector2f(0.0f, 0.0f),float radius = 0.0f, float mass = 2000000);
	void removeLastBlackHole();
	void removeAllBlackHoles();
	*/


protected :
	/*
	void calculateMouseAttraction(bool LMB, Vector2f mousePos);
	void calculateBlackHoleAttraction(int index);
	void calculateDragForce();
	*/
	int NUM_PARTICLES;

	Particles<MAX_NUM_PARTICLES> particles;
	//vector<BlackHole> blackHoles;

	// Pour afficher les particules en tant que pixel, d'abord créer une image qui se comporte en array de pixel,
	// puis le mettre dans une texture que l'on met également dans un sprite pour enfin pouvoir l'afficher.
	Image image;
	Texture texture;
	Sprite sprite;
};

