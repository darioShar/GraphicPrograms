#include "CorpsCeleste.h"

#define SCREEN_REBOUNCE

CorpsCeleste::CorpsCeleste(PlanetID &p, Vector2f &pos, bool movable, Vector2f &speed) 
	:	m_weight(p.weight)
	,	m_color(p.color)
	,	m_name(p.name)
	,	m_texture(p.texture)
	,	m_forceApplied(Vector2f(0.0f, 0.0f))
	,	m_acceleration(Vector2f(0.0f, 0.0f))
	,	m_speed(speed*static_cast<float>(movable))
	,	m_movable(movable)
{
	setRadius(p.radius);
	setOrigin(getRadius(), getRadius());
	setPosition(pos);
	setFillColor(m_color);
}

void CorpsCeleste::applyForce(Vector2f force)
{
	m_forceApplied = force;
	applyAcceleration(m_forceApplied / m_weight);
}

void CorpsCeleste::applyAcceleration(Vector2f acceleration)
{
	m_acceleration = acceleration;
}

void CorpsCeleste::addForce(Vector2f force)
{
	m_forceApplied += force;
	addAcceleration(m_forceApplied / m_weight);
}

void CorpsCeleste::addAcceleration(Vector2f acceleration)
{
	m_acceleration += acceleration;
}

void CorpsCeleste::clearForces()
{
	m_forceApplied = Vector2f(0.0f, 0.0f);
	m_acceleration = Vector2f(0.0f, 0.0f);
}

Vector2f CorpsCeleste::getCenter() const
{
	Vector2f center = getPosition();
	return center;
}



void CorpsCeleste::moveBody(float time)
{
	if (m_movable) {
		Vector2f previousPos = getCenter();

		move(m_speed * time + 0.5f*m_acceleration*time*time);
		m_speed = (getCenter() - previousPos) / time;
		// Rebounce
#ifdef SCREEN_REBOUNCE
		if ((getCenter() + m_speed*time).x < 0 || (getCenter() + m_speed*time).x > WIDTH) {
			m_speed.x *= -1;
		}
		if ((getCenter() + m_speed*time).y < 0 || (getCenter() + m_speed*time).y > HEIGHT) {
			m_speed.y *= -1;
		}
#endif
	}
}


void CorpsCeleste::applyTexture() {
	setTexture(&m_texture);
}