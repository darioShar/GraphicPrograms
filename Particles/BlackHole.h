#pragma once
#include <SFML\system.hpp>

using namespace sf;

struct BlackHole
{
	float mass;
	float radius;
	Vector2f position;

	BlackHole(Vector2f p = Vector2f(0.0f, 0.0f), float r = 0.0f, float m = 2000000.0f)
		:	mass(m)
		,	position(p)
		,	radius(r)
	{
	}
};

