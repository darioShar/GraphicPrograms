#pragma once
#include <Windows.h>
#include <SFML\Graphics.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "Vue.h"

using namespace sf;
using namespace std;

constexpr int WIDTH = 1600;
constexpr int HEIGHT = 900;
constexpr DATA_TYPE ASPECT_RATIO = (DATA_TYPE)WIDTH / (DATA_TYPE)HEIGHT;
constexpr int NUM_PIXEL = WIDTH * HEIGHT;

struct ComplexMandelbrot {
	DATA_TYPE real = 0.0;
	DATA_TYPE imaginary = 0.0;

};

class Plane : public RenderWindow
{
	int ITERATIONS = 500;

	Vue	vue;

	ComplexMandelbrot *complexNumbers;
	int *screen;
	Image complexPlane;
	Texture texture;
	Sprite sprite;


public:
	Plane(Vue);
	~Plane();

	void setWindow(Vue);
	void resetComplexNumbers();
	void drawPlane();
	ComplexMandelbrot* getComplexNumbers();
	int* getScreenPointer();
	int getNumIterations();
	void setNumIterations(int);
};

