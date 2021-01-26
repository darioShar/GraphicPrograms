#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "CorpsCeleste.h"
#include "Galaxy.h"

#define FRAME_TIME_MS 1

#define TIME_SCALE 1.0f


int main()
{
	SolarSystem solarSystem;
	Galaxy galaxy(sf::Color::Black);
	galaxy.addBody(solarSystem.Sun);
	galaxy.addBody(solarSystem.Mercury);
	galaxy.addBody(solarSystem.Venus);
	galaxy.addBody(solarSystem.Earth);
	galaxy.addBody(solarSystem.Mars);
	galaxy.addBody(solarSystem.Jupiter);
	galaxy.addBody(solarSystem.Saturn);
	galaxy.addBody(solarSystem.Uranus);
	galaxy.addBody(solarSystem.Neptune);

	galaxy.setTextures(true);
	while (galaxy.isOpen())
	{
		sf::Event event;

		Clock clock;
		float time = 0.0000000001f;
		while (galaxy.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				galaxy.close();

			/*Si on veut créer un objet de très grande masse avec le curseur*/
			/*
			Vector2f cursorPos;
			
			if (event.type == sf::Event::MouseButtonPressed) {
				cursorPos.x = (float)sf::Mouse::getPosition().x;
				cursorPos.y = (float)sf::Mouse::getPosition().y;
				galaxy.addBody(CorpsCeleste(PlanetID(20.0f, 2e33f), cursorPos));
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				galaxy.removeBody();
			}
			if (event.type == sf::Event::MouseMoved) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					cursorPos.x = (float)sf::Mouse::getPosition().x;
					cursorPos.y = (float)sf::Mouse::getPosition().y;
					galaxy.removeBody();
					galaxy.addBody(CorpsCeleste(PlanetID(20.0f, 2e33f), cursorPos));
				}
			}
			*/
		}

		galaxy.applyGravity(GravityType::MostMassiveBodyOnly, time * TIME_SCALE);
		galaxy.rotateBodies(15.0f * TIME_SCALE);

		galaxy.clear();
		galaxy.drawGalaxy();
		galaxy.display();
		Sleep(FRAME_TIME_MS);
		time = clock.restart().asSeconds();
	} 

	return 0;
}