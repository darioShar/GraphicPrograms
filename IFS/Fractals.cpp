// Fractals.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <random>
#include "ComplexPlane.h"

#define INIT_POINTS 1

int main()
{
	RenderWindow window(VideoMode(1600, 900), "Fractal");
	Event evnt;

	vector<complex<float>> initPoints(INIT_POINTS);
	for (int i = 0; i < INIT_POINTS; i++) {
		float x = ((float)rand() / RAND_MAX) * 0.2f + 0.75f;
		x = (x - 0.5f) * 2.0f;
		float y = ((float)rand() / RAND_MAX) * 0.2f - 0.9f;
		y = (y - 0.5f) * 2.0f;
		initPoints[i] = complex<float>(x, y);
	}

	Vue v;
	v.zoom(0.45f);

	ComplexPlane cp(window, initPoints, v);
	// Pas mal
	/*
	cp.addFractalFunction(Homothetie(complex<float>(0.4f, 0.4f), complex<float>(-0.22f, 0.4928f)));
	cp.addFractalFunction(Homothetie(complex<float>(0.6f, 0.0f), complex<float>(0.2528f, 0.16f)));
	cp.addFractalFunction(Homothetie(complex<float>(-0.16f, 0.7f), complex<float>(-0.37852, 0.545f)));
	cp.addFractalFunction(Homothetie(complex<float>(-0.7f, -0.25f), complex<float>(0.69f, 0.5f)));
	*/
	// Julia (point fixe sur x^2)
	/*
	cp.addFractalFunction(Homothetie(complex<float>(0.4f, 0.6f), complex<float>(-0.447f, 0.298f)));
	cp.addFractalFunction(Homothetie(complex<float>(0.6f, 0.3f), complex<float>(0.0f, -0.298f)));
	cp.addFractalFunction(Homothetie(complex<float>(-0.25, 0.7f), complex<float>(0.0f, 0.0f)));
	*/
	/*
	cp.addFractalFunction(Homothetie(complex<float>(-0.9f, 0.1f), complex<float>(-0.447f, 0.298f)));
	cp.addFractalFunction(Homothetie(complex<float>(0.6f, 0.8f), complex<float>(0.0f, -0.298f)));
	cp.addFractalFunction(Homothetie(complex<float>(-0.7f, -0.25f), complex<float>(0.69f, 0.5f)));
	*/


	while (window.isOpen()) {
		while (window.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) {
				window.close();
			}
		}
		
		window.clear(Color::Blue);
		cp.iterate(8);
		cp.draw();
		window.display();
	}
    return 0;
}

