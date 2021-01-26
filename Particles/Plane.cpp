#include "Plane.h"
#include "utils.h"
#include <thread>
#include <mutex>

#define INITIAL_DISTANCE 1.0f
#define DRAG 4
#define MOUSE_FORCE 750000.0f
#define SCALE_FACTOR 1
#define SECURITY_DISTANCE_SQUARED 900.0f
#define COLLISION_DETECTION
#undef COLLISION_DETECTION

Plane::Plane() 
	:	RenderWindow(VideoMode(PLANE_WIDTH, PLANE_HEIGHT), "Particles")
{
	/*	ATTENTION !!!!!!!!!!!!!
		SI NUM_PARTICLES N'EST PAS UN CARRE PARFAIT : PROBLEME !!!
	*/
	/*particles.resize(NUM_PARTICLES);
	vector<Particle>::iterator it = particles.begin();
	for (int i = 0; i < INITIAL_PARTICLE_HEIGHT; i++) {
		for (int j = 0; j < INITIAL_PARTICLE_WIDTH; j++) {
			Particle p;
			p.setPosition(Vector2f((float)j*INITIAL_DISTANCE, (float)i*INITIAL_DISTANCE));
			p.setMass(10);
			*it = p;
			it++;
		}
	}*/


	Image imageFile;
	imageFile.loadFromFile("Image\\image.png");
	Vector2u size = imageFile.getSize();
	/*PLANE_WIDTH = size.x;
	PLANE_HEIGHT = size.y;*/

	int k = 0;
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			if (imageFile.getPixel(i, j) == Color::Black) {
				if (k >= MAX_NUM_PARTICLES) {
					image.create(PLANE_WIDTH, PLANE_HEIGHT);
					NUM_PARTICLES = MAX_NUM_PARTICLES;
					return;
				}
				particles.position[k].x = i;
				particles.position[k].y = j;
				particles.speed[k].x = 0.0f;
				particles.speed[k].y = 0.0f;
				k++;
			}
		}
	}

	NUM_PARTICLES = k;
	/*
	int count = 0;
	for (int i = 0; i < INITIAL_PARTICLE_HEIGHT; i++) {
		for (int j = 0; j < INITIAL_PARTICLE_WIDTH; j++) {
			particles.position[count].x = j*INITIAL_DISTANCE;
			particles.position[count].y = i*INITIAL_DISTANCE;
			particles.speed[count].x = 0.0f;
			particles.speed[count].y = 0.0f;
			count++;
		}
	}
	*/

	image.create(PLANE_WIDTH, PLANE_HEIGHT);
}


Plane::~Plane()
{
}
/*
void Plane::addBlackHole(Vector2f position, float radius, float mass) {
	blackHoles.push_back(BlackHole(position, radius, mass));
}
void Plane::removeLastBlackHole() {
	if (!blackHoles.empty()) {
		blackHoles.pop_back();
	}
}

void Plane::removeAllBlackHoles() {
	blackHoles.clear();
}*/

void Plane::updateParticlesPosition(float dt, bool LMB, Vector2f mousePos) {
	// Still need to optimize with mutex and shit.
	/*
	thread t_mouse;
	vector<thread> t_blackHoles;
	thread t_drag;

	t_mouse = thread(&Plane::calculateMouseAttraction, this, LMB, mousePos);

	for (int i = 0; i < blackHoles.size(); i++) {
		t_blackHoles.push_back(thread(&Plane::calculateBlackHoleAttraction, this, i));
	}

	t_drag = thread(&Plane::calculateDragForce, this);

	auto blackHoleThread = t_blackHoles.begin();
	while (blackHoleThread != t_blackHoles.end())
	{
		blackHoleThread->join();
		blackHoleThread++;
	}
	t_mouse.join();
	t_drag.join();

	*/
	
	/*
	for (int i = 0; i < blackHoles.size(); i++) {
		calculateBlackHoleAttraction(i);
	}
	*/

	if (LMB) {
		float previousPosX;
		float previousPosY;
		float dx;
		float dy;
		float distanceSquared;
		for (int i = 0; i < NUM_PARTICLES; i++) {
			previousPosX = particles.position[i].x;
			previousPosY = particles.position[i].y;

			dx = mousePos.x - particles.position[i].x;
			dy = mousePos.y - particles.position[i].y;
			distanceSquared = dx*dx + dy*dy;

			if (distanceSquared > SECURITY_DISTANCE_SQUARED) {
				particles.position[i].x += (particles.speed[i].x * dt)
					+ (0.5f*(
					(dx * (float)MOUSE_FORCE / (distanceSquared))
						- (particles.speed[i].x * (float)DRAG)
						) * (dt * dt));
				particles.speed[i].x = (particles.position[i].x - previousPosX) / dt;

				particles.position[i].y += (particles.speed[i].y * dt)
					+ (0.5f*(
					(dy * (float)MOUSE_FORCE / (distanceSquared))
						- (particles.speed[i].y * (float)DRAG)
						) * (dt * dt));
				particles.speed[i].y = (particles.position[i].y - previousPosY) / dt;
			}
			else {
				particles.position[i].x += (particles.speed[i].x * dt)
					+ (0.5f*(
						-(particles.speed[i].x * (float)DRAG)
						) * (dt * dt));
				particles.speed[i].x = (particles.position[i].x - previousPosX) / dt;

				particles.position[i].y += (particles.speed[i].y * dt)
					+ (0.5f*(
						-(particles.speed[i].y * (float)DRAG)
						) * (dt * dt));
				particles.speed[i].y = (particles.position[i].y - previousPosY) / dt;
			}
		}
	}
	else {
		float previousPosX;
		float previousPosY;
		float dx;
		float dy;
		float distanceSquared;
		for (int i = 0; i < NUM_PARTICLES; i++) {
			previousPosX = particles.position[i].x;
			previousPosY = particles.position[i].y;

			dx = mousePos.x - particles.position[i].x;
			dy = mousePos.y - particles.position[i].y;
			distanceSquared = dx*dx + dy*dy;

			particles.position[i].x += (particles.speed[i].x * dt)
				+ (0.5f*(
					- (particles.speed[i].x * (float)DRAG)
					) * (dt * dt));
			particles.speed[i].x = (particles.position[i].x - previousPosX) / dt;

			particles.position[i].y += (particles.speed[i].y * dt)
				+ (0.5f*(
					- (particles.speed[i].y * (float)DRAG)
					) * (dt * dt));
			particles.speed[i].y = (particles.position[i].y - previousPosY) / dt;
		}
	}

										 // Collision detection :
#ifdef COLLISION_DETECTION
		if (particles[i].getPosition().x < 0.0f) {
			particles[i].setSpeed(Vector2f(-particles[i].getSpeed().x, particles[i].getSpeed().y));
			particles[i].setPosition(Vector2f(0.0f, particles[i].getPosition().y));
		}
		else if (particles[i].getPosition().x >= PLANE_WIDTH) {
			particles[i].setSpeed(Vector2f(-particles[i].getSpeed().x, particles[i].getSpeed().y));
			particles[i].setPosition(Vector2f((float)PLANE_WIDTH - 1.0f, particles[i].getPosition().y));
		}

		if (particles[i].getPosition().y < 0.0f) {
			particles[i].setSpeed(Vector2f(particles[i].getSpeed().x, -particles[i].getSpeed().y));
			particles[i].setPosition(Vector2f(particles[i].getPosition().x, 0.0f));
		}
		else if (particles[i].getPosition().y >= PLANE_HEIGHT) {
			particles[i].setSpeed(Vector2f(particles[i].getSpeed().x, -particles[i].getSpeed().y));
			particles[i].setPosition(Vector2f(particles[i].getPosition().x, (float)PLANE_HEIGHT - 1.0f));
		}
#endif
}

/*
void Plane::calculateMouseAttraction(bool LMB, Vector2f mousePos) {
	if (LMB) {
		float distance;
		for (int i(0); i < NUM_PARTICLES; i++) // now, each frame, we want to update each particle's position according to the newton's law, color according to its speed, and maybe make it collide with walls (this for loop is executed 1 billion times each frame)
		{
			//particles[i].addForce(Vector2f(0,GRAVITY)) ; // example for adding gravity force
			// Don't apply force of too close to mouse : would accelerate too abruptly
			distance = DistanceScaled(mousePos, particles[i].getPosition(), SCALE_FACTOR);
			if (distance > SECURITY_DISTANCE) {
				particles[i].addForce(((mousePos - particles[i].getPosition()) * (float)(MOUSE_FORCE / (distance*distance)))); // if the user clicks we add a force proportionnal to the inverse of the distance squared
			}
		}
	}
}
*/
/*
void Plane::calculateBlackHoleAttraction(int index) {
	if (index >= 0 && index < blackHoles.size()) {
		float distance;
		for (int i = 0; i < NUM_PARTICLES; i++) {
			distance = DistanceScaled(blackHoles[index].position, particles[i].getPosition(), SCALE_FACTOR);
			if (distance > SECURITY_DISTANCE) {
				particles[i].addForce(((blackHoles[index].position - particles[i].getPosition()) * (float)(blackHoles[index].mass / (distance*distance))));
			}
		}
	}
}

*/
/*
void Plane::calculateDragForce() {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].addForce(-particles[i].getSpeed()*(float)DRAG); // we add a drag force proportionnal to the speed
	}
}
*/

void Plane::centerParticlesPosition()
{
	Image imageFile;
	imageFile.loadFromFile("Image\\image.png");
	Vector2u size = imageFile.getSize();
	/*PLANE_WIDTH = size.x;
	PLANE_HEIGHT = size.y;*/

	int k = 0;
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			if (imageFile.getPixel(i, j) == Color::Black) {
				if (k >= MAX_NUM_PARTICLES) {
					image.create(PLANE_WIDTH, PLANE_HEIGHT);
					NUM_PARTICLES = MAX_NUM_PARTICLES;
					return;
				}
				particles.position[k].x = i;
				particles.position[k].y = j;
				particles.speed[k].x = 0.0f;
				particles.speed[k].y = 0.0f;
				k++;
			}
		}
	}

	NUM_PARTICLES = k;

	/*
	int k = 0;
	float xpos = (PLANE_WIDTH - INITIAL_PARTICLE_WIDTH) / 2.0f;
	float ypos = (PLANE_HEIGHT - INITIAL_PARTICLE_HEIGHT) / 2.0f;
	for (int i = 0; i < INITIAL_PARTICLE_HEIGHT; i++) {
		for (int j = 0; j < INITIAL_PARTICLE_WIDTH; j++) {
			particles.position[k].x = xpos + j;
			particles.position[k].y = ypos + i;
			particles.speed[k].x = 0.0f;
			particles.speed[k].y = 0.0f;
			k++;
		}
	}
	*/
}


void Plane::clearPlane() {
	clear();
	image.create(PLANE_WIDTH, PLANE_HEIGHT, Color::White);
}

void Plane::drawPlane()
{
	for (int i = 0; i < NUM_PARTICLES; i++) {
		// we want to check that each pixel is within boundaries
		if (particles.position[i].x > 0.0f && particles.position[i].x < ((float)PLANE_WIDTH - 1.0f) && particles.position[i].y > 0.0f && particles.position[i].y < ((float)PLANE_HEIGHT - 1.0f)) {
			image.setPixel(particles.position[i].x, particles.position[i].y, Color(255, clamp(255-Norm(particles.speed[i]), 0, 255), 0));
		}
	}
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	draw(sprite);

	/*
	for (int i = 0; i < blackHoles.size(); i++) {
		if (blackHoles[i].radius > 0.0f) {
			CircleShape circle;
			circle.setRadius(blackHoles[i].radius);
			circle.setOrigin(blackHoles[i].radius, blackHoles[i].radius);
			circle.setPosition(blackHoles[i].position);
			circle.setFillColor(Color::Black);
			draw(circle);
		}
	}
	*/
}
