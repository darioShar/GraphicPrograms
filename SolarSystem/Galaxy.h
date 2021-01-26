#pragma once
#include <SFML\Graphics.hpp>
#include "CorpsCeleste.h"

using namespace sf;

enum GravityType {
	NoGravity,
	MostMassiveBodyOnly,
	All
};

class Galaxy : public RenderWindow
{
public:
	Galaxy(Color background = Color::Black);
	~Galaxy();

	void setTextures(bool);
	void addBody(CorpsCeleste &body);
	void removeBody();
	void applyGravity(GravityType, float time);
	void rotateBodies(float rotation);
	void drawGalaxy();

protected :
	std::vector<CorpsCeleste> m_Bodies;
	int m_indexMostMassiveBody;

	bool m_showTextures;
	Texture t_milkyWay;
	Sprite milkyWay;
};

