#include <SFML\Graphics.hpp>
#include <Windows.h>
#include "Plane.h"

using namespace sf;
using namespace std;

#define BLACK_HOLE_RADIUS 10.0f
#define BLACK_HOLE_MASS 2000000.0f

int main()
{
	Plane plane;

	Clock deltaTime; // SFML clock to keep track of our framerate
	float dt = 0.00000001; // the time a frame requires to be drawn (=1/framerate). I set it at 0.0000001 to avoid getting undefined behaviors during the very first frame

	bool LMB = false; // is left mouse button hit ?

	while (plane.isOpen())
	{
		sf::Event event;
		while (plane.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				plane.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::R)) {
			plane.centerParticlesPosition();
		}

		Vector2f mousePos = (Vector2f(Mouse::getPosition(plane).x, Mouse::getPosition(plane).y) /* - Vector2f(width / 2, height / 2)*/); // we store the current mouse position in this variable

		if (Mouse::isButtonPressed(Mouse::Left))
			LMB = true;
		else
			LMB = false;

		/*
		if (Mouse::isButtonPressed(Mouse::Right)) {
			plane.addBlackHole(mousePos, BLACK_HOLE_RADIUS, BLACK_HOLE_MASS);
		}

		if (Keyboard::isKeyPressed(Keyboard::B)) {
			plane.removeLastBlackHole();
		}
		*/

		plane.updateParticlesPosition(dt, LMB, mousePos);
		
		plane.clearPlane();
		plane.drawPlane();
		plane.display();
		dt = deltaTime.restart().asSeconds(); // measure the time it took to complete all the calculations for this particular frame (dt=1/framerate) 
	}

	return 0;
}