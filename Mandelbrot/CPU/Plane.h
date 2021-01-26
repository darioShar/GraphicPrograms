#pragma once
#include <Windows.h>
#include <SFML\Graphics.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "Vue.h"

using namespace sf;
using namespace std;

constexpr int NUM_THREAD = 6;

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;
constexpr DATA_TYPE ASPECT_RATIO = (DATA_TYPE)WIDTH / (DATA_TYPE)HEIGHT;
constexpr int NUM_PIXEL = WIDTH * HEIGHT;

 struct ComplexMandelbrot {
	 DATA_TYPE real = 0.0;
	 DATA_TYPE imaginary = 0.0;

};

class Plane : public RenderWindow
{
	int ITERATIONS = 1;

	Vue	vue;

	ComplexMandelbrot *complexNumbers;
	int *screen;
	Image complexPlane;
	Texture texture;
	Sprite sprite;

	thread _thread[NUM_THREAD];
	

public:
	Plane(Vue);
	~Plane();

	void setWindow(Vue);
	
	void resetComplexNumbers();
	void resetThemComplex(int lineBegin, int lineEnd);

	/*
	void createMandelbrotSet();
	void calculateMandelbrot(int indexBegin, int indexEnd);
	*/
	void leDessin(int, int);
	
	void drawPlane();
};

