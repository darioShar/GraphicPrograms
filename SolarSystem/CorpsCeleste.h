#pragma once
#include <SFML\Graphics.hpp>
#include <string>


#define WIDTH 1920
#define HEIGHT 1080

#define GRAVITY_CONSTANT 6.67408e-11f


#define SUN_RADIUS		55.0f
#define SUN_WEIGHT		1.9891e30f
#define SUN_COLOR		Color::Yellow
#define SUN_X			WIDTH/2
#define SUN_Y			540.0f
#define SUN_TEXTURE		"Textures\\Sun.jpg"

#define MERCURY_RADIUS	5.0f
#define MERCURY_WEIGHT	3.3022e23f
#define MERCURY_COLOR	Color(165, 42, 42, 255)
#define MERCURY_X		WIDTH/2
#define MERCURY_Y		605.0f
#define MERCURY_INITIAL_VELOCITY Vector2f(8.15e9f, 0.0f)
#define MERCURY_TEXTURE		"Textures\\Mercury.jpg"


#define VENUS_RADIUS	7.0f
#define VENUS_WEIGHT	4.8685e24f
#define VENUS_COLOR		Color(255, 127, 80, 255)
#define VENUS_X			WIDTH/2
#define VENUS_Y			621.0f
#define VENUS_INITIAL_VELOCITY Vector2f(8.15e9f, 0.0f)
#define VENUS_TEXTURE		"Textures\\Venus.jpg"


#define EARTH_RADIUS	10.0f
#define EARTH_WEIGHT	5.9736e24f
#define EARTH_COLOR		Color::Blue
#define EARTH_X			WIDTH/2
#define EARTH_Y			652.0f
#define EARTH_INITIAL_VELOCITY Vector2f(8.15e9f, 0.0f)
#define EARTH_TEXTURE		"Textures\\Earth.jpg"


#define MARS_RADIUS		9.0f
#define MARS_WEIGHT		6.4185e23f
#define MARS_COLOR		Color::Red
#define MARS_X			WIDTH/2
#define MARS_Y			685.0f
#define MARS_INITIAL_VELOCITY Vector2f(8.15e9f, 0.0f)
#define MARS_TEXTURE		"Textures\\Mars.jpg"


#define JUPITER_RADIUS	30.0f
#define JUPITER_WEIGHT	1.8986e27f
#define JUPITER_COLOR	Color(255, 140, 0, 255)
#define JUPITER_X		WIDTH/2
#define JUPITER_Y		310.0f
#define JUPITER_INITIAL_VELOCITY Vector2f(-8.15e9f, 0.0f)
#define JUPITER_TEXTURE		"Textures\\Jupiter.jpg"


#define SATURN_RADIUS	26.0f
#define SATURN_WEIGHT	5.6846e23f
#define SATURN_COLOR	Color(218, 165, 32, 255)
#define SATURN_X		WIDTH/2
#define SATURN_Y		220.0f
#define SATURN_INITIAL_VELOCITY Vector2f(-9e9f, 0.0f)
#define SATURN_TEXTURE		"Textures\\Saturn.jpg"


#define URANUS_RADIUS	23.0f
#define URANUS_WEIGHT	8.6810e25f
#define URANUS_COLOR	Color(30, 144, 255, 255)
#define URANUS_X		WIDTH/2
#define URANUS_Y		120.0f
#define URANUS_INITIAL_VELOCITY Vector2f(-9.5e9f, 0.0f)
#define URANUS_TEXTURE		"Textures\\Uranus.jpg"


#define NEPTUNE_RADIUS	20.0f
#define NEPTUNE_WEIGHT	10.243e25f
#define NEPTUNE_COLOR	Color(100, 149, 237, 255)
#define NEPTUNE_X		WIDTH/2
#define NEPTUNE_Y		50.0f
#define NEPTUNE_INITIAL_VELOCITY Vector2f(-1e10f, 0.0f)
#define NEPTUNE_TEXTURE		"Textures\\Neptune.jpg"



using namespace sf;

struct PlanetID {
	PlanetID(float r = 50.0f, float w = 1.0f, sf::Color c = Color::Red, std::string n = "No Name", std::string textureFileName = "") 
		:	radius(r)
		,	weight(w)
		,	color(c)
		,	name(n)
	{
		if (!textureFileName.empty()) {
			if (texture.loadFromFile(textureFileName)) {
				color = Color::White;
			}
		}
	}
	

	float		radius;
	float		weight;
	Color		color;
	std::string name;
	Texture		texture;
};


class CorpsCeleste : public CircleShape
{
public :
	CorpsCeleste(PlanetID &p = PlanetID(), Vector2f &pos = Vector2f(0.0f,0.0f), bool movable = true, Vector2f &initialSpeed = Vector2f(0.0f, 0.0f));
	void applyForce(Vector2f force);
	void applyAcceleration(Vector2f acceleration);
	void addForce(Vector2f force);
	void addAcceleration(Vector2f acceleration);
	void clearForces();
	void moveBody(float time);

	void applyTexture();

	Vector2f getCenter() const;
	bool isMovable() const { return m_movable; }
	float weight() const { return m_weight; }
	std::string name() const { return m_name; }

protected :
	float		m_weight;
	Color		m_color;
	Texture		m_texture;
	std::string m_name;
	bool		m_movable;
	Vector2f	m_forceApplied;
	Vector2f	m_acceleration;
	Vector2f	m_speed;
};

struct SolarSystem {

	CorpsCeleste Sun		= CorpsCeleste(PlanetID(SUN_RADIUS, SUN_WEIGHT, SUN_COLOR, "Sun", SUN_TEXTURE), Vector2f(SUN_X, SUN_Y), false);

	CorpsCeleste Mercury	= CorpsCeleste(PlanetID(MERCURY_RADIUS, MERCURY_WEIGHT, MERCURY_COLOR, "Mercury", MERCURY_TEXTURE), Vector2f(MERCURY_X, MERCURY_Y), true, MERCURY_INITIAL_VELOCITY);

	CorpsCeleste Venus		= CorpsCeleste(PlanetID(VENUS_RADIUS, VENUS_WEIGHT, VENUS_COLOR, "Venus", VENUS_TEXTURE), Vector2f(VENUS_X, VENUS_Y), true, VENUS_INITIAL_VELOCITY);

	CorpsCeleste Earth		= CorpsCeleste(PlanetID(EARTH_RADIUS, EARTH_WEIGHT, EARTH_COLOR, "Earth", EARTH_TEXTURE), Vector2f(EARTH_X, EARTH_Y), true, EARTH_INITIAL_VELOCITY);

	CorpsCeleste Mars		= CorpsCeleste(PlanetID(MARS_RADIUS, MARS_WEIGHT, MARS_COLOR, "Mars", MARS_TEXTURE), Vector2f(MARS_X, MARS_Y), true, MARS_INITIAL_VELOCITY);

	CorpsCeleste Jupiter	= CorpsCeleste(PlanetID(JUPITER_RADIUS, JUPITER_WEIGHT, JUPITER_COLOR, "Jupiter", JUPITER_TEXTURE), Vector2f(JUPITER_X, JUPITER_Y), true, JUPITER_INITIAL_VELOCITY);

	CorpsCeleste Saturn		= CorpsCeleste(PlanetID(SATURN_RADIUS, SATURN_WEIGHT, SATURN_COLOR, "Saturn", SATURN_TEXTURE), Vector2f(SATURN_X, SATURN_Y), true, SATURN_INITIAL_VELOCITY);

	CorpsCeleste Uranus		= CorpsCeleste(PlanetID(URANUS_RADIUS, URANUS_WEIGHT, URANUS_COLOR, "Uranus", URANUS_TEXTURE), Vector2f(URANUS_X, URANUS_Y), true, URANUS_INITIAL_VELOCITY);

	CorpsCeleste Neptune	= CorpsCeleste(PlanetID(NEPTUNE_RADIUS, NEPTUNE_WEIGHT, NEPTUNE_COLOR, "Neptune", NEPTUNE_TEXTURE), Vector2f(NEPTUNE_X, NEPTUNE_Y), true, NEPTUNE_INITIAL_VELOCITY);

};