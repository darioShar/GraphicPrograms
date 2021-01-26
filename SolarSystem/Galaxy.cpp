#include "Galaxy.h"
#include "utils.h"


#define SIZE_STARS 1

#define GRAVITY_CONSTANT 6.67408e-11f
#define SECURITY_DISTANCE 2.0f

using namespace std;


Galaxy::Galaxy(Color background)
	:	RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Solar System")
	,	m_indexMostMassiveBody(0)
	,	m_showTextures(false)
{
	clear(background);
	setTextures(m_showTextures);
}


Galaxy::~Galaxy()
{
}


void Galaxy::setTextures(bool showTextures) {
	m_showTextures = showTextures;

	if (m_showTextures) {
		if (t_milkyWay.loadFromFile("Textures\\StarsMilkyway.jpg")) {
			milkyWay.setTexture(t_milkyWay);
		}
		for (int i = 0; i < m_Bodies.size(); i++) {
			m_Bodies[i].applyTexture();
		}
	}

	drawGalaxy();
	
}

void Galaxy::addBody(CorpsCeleste &body)
{
	m_Bodies.push_back(body);
	if (body.weight() > m_Bodies[m_indexMostMassiveBody].weight())
		m_indexMostMassiveBody = m_Bodies.size() - 1;
}

void Galaxy::removeBody() {
	if (m_Bodies.size() > 0) {
		m_Bodies.pop_back();
		m_indexMostMassiveBody = 0;
		float maxWeight = 0.0f;
		for (int i = 0; i < m_Bodies.size(); i++) {
			if (m_Bodies[i].weight() > maxWeight) {
				m_indexMostMassiveBody = i;
				maxWeight = m_Bodies[i].weight();
			}
		}

		// reset acceleration
		for (int i = 0; i < m_Bodies.size(); i++) {
			m_Bodies[i].applyAcceleration(Vector2f(0.0f, 0.0f));
		}
	}
}

void Galaxy::applyGravity(GravityType gravityType, float time) {
	if (gravityType == GravityType::NoGravity)
		return;

	Vector2f space;
	float distance;
	if (gravityType == GravityType::MostMassiveBodyOnly) {
		for (int i = 0; i < m_indexMostMassiveBody; i++) {
			if (m_Bodies[i].isMovable()) {
				space = m_Bodies[m_indexMostMassiveBody].getCenter() - m_Bodies[i].getCenter();
				distance = Norm(space);
				if (distance > SECURITY_DISTANCE) {
					m_Bodies[i].applyAcceleration((GRAVITY_CONSTANT * space * m_Bodies[m_indexMostMassiveBody].weight() / (distance*distance)));
				}
				m_Bodies[i].moveBody(time);
				m_Bodies[i].clearForces();
			}
		}
		for (int i = m_indexMostMassiveBody + 1; i < m_Bodies.size(); i++) {
			if (m_Bodies[i].isMovable()) {
				space = m_Bodies[m_indexMostMassiveBody].getCenter() - m_Bodies[i].getCenter();
				distance = Norm(space);
				if (distance > SECURITY_DISTANCE) {
					m_Bodies[i].applyAcceleration((GRAVITY_CONSTANT * space * m_Bodies[m_indexMostMassiveBody].weight() / (distance*distance)));
				}
				m_Bodies[i].moveBody(time);
				m_Bodies[i].clearForces();
			}
		}
	}
	else if (gravityType == GravityType::All) {
		// bon la on va pas vérifier si l'objet est movable ou pas, bien qu'il fuadrait, mais la c'est chiant.

		// reset all acceleration
		for (int i = 0; i < m_Bodies.size(); i++) {
			m_Bodies[i].clearForces();
		}
		for (int i = 0; i < m_Bodies.size(); i++) {
			for (int j = i + 1; j < m_Bodies.size(); j++) {
				space = m_Bodies[j].getCenter() - m_Bodies[i].getCenter();
				distance = Norm(space);
				if (distance > SECURITY_DISTANCE) {
					m_Bodies[i].addAcceleration(GRAVITY_CONSTANT * space * m_Bodies[m_indexMostMassiveBody].weight() / (distance*distance));
					m_Bodies[j].addAcceleration(GRAVITY_CONSTANT * -space * m_Bodies[m_indexMostMassiveBody].weight() / (distance*distance));
				}
			}
			m_Bodies[i].moveBody(time);
			m_Bodies[i].clearForces();
		}
	}
}

void Galaxy::rotateBodies(float rotation)
{
	for (int i = 0; i < m_Bodies.size(); i++) {
		m_Bodies[i].rotate(rotation /m_Bodies[i].getRadius());
	}
}


void Galaxy::drawGalaxy() {
	if (m_showTextures) {
		draw(milkyWay);
	}
	for (int i = 0; i < m_Bodies.size(); i++) {
		draw(m_Bodies[i]);
	}
}
