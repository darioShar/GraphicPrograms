#include "Plane.h"


Plane::Plane(Vue v)
	:	RenderWindow(VideoMode(WIDTH, HEIGHT), "Mandelbrot Set", Style::Fullscreen)
	,	vue(v)
	,	complexNumbers(nullptr)
	,	screen(nullptr)
{
	
	complexNumbers = new(std::nothrow) ComplexMandelbrot[NUM_PIXEL];
	screen = new(std::nothrow) int[NUM_PIXEL];

	if (complexNumbers == nullptr) cout << "complexNumbers : Allocation failed" << endl;
	if (screen == nullptr) cout << "complexNumbers : Allocation failed" << endl;

	DATA_TYPE dx = (vue.xmax - vue.xmin) / WIDTH;
	DATA_TYPE dy = (vue.ymax - vue.ymin) / HEIGHT;
	DATA_TYPE Xpos = vue.xmin;
	DATA_TYPE Ypos = vue.ymax;

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			complexNumbers[i*WIDTH + j].real = Xpos;
			complexNumbers[i*WIDTH + j].imaginary = Ypos;
			Xpos += dx;
		}
		Xpos = vue.xmin;
		Ypos -= dy;
	}
}



Plane::~Plane()
{
	delete[] complexNumbers;
	delete[] screen;
}


void Plane::resetComplexNumbers() {
	for (int i = 0; i < NUM_THREAD; i++) {
		_thread[i] = thread(&Plane::resetThemComplex, this, i * HEIGHT / NUM_THREAD, (i + 1) * HEIGHT / NUM_THREAD);
		SetThreadPriority(_thread[i].native_handle(), THREAD_PRIORITY_HIGHEST);
	}

	for (int i = 0; i < NUM_THREAD; i++) {
		_thread[i].join();
	}
}

void Plane::resetThemComplex(int lineBegin, int lineEnd) {
	DATA_TYPE dx = (vue.xmax - vue.xmin) / WIDTH;
	DATA_TYPE dy = (vue.ymax - vue.ymin) / HEIGHT;
	DATA_TYPE Xpos = vue.xmin;
	DATA_TYPE Ypos = vue.ymax - (lineBegin * dy);

	for (int i = lineBegin; i < lineEnd; i++) {
		for (int j = 0; j < WIDTH; j++) {
			complexNumbers[i*WIDTH + j].real = Xpos;
			complexNumbers[i*WIDTH + j].imaginary = Ypos;
			Xpos += dx;
		}
		Xpos = vue.xmin;
		Ypos -= dy;
	}
}

/*
void Plane::createMandelbrotSet() {
	for (int i = 0; i < NUM_THREAD; i++) {
		_thread[i] = thread(&Plane::calculateMandelbrot, this, i * NUM_PIXEL / NUM_THREAD, (i + 1) * NUM_PIXEL / NUM_THREAD);
		SetThreadPriority(_thread[i].native_handle(), THREAD_PRIORITY_HIGHEST);
	}

	for (int i = 0; i < NUM_THREAD; i++) {
		_thread[i].join();
	}
}

void Plane::calculateMandelbrot(int indexBegin, int indexEnd) {
	
	DATA_TYPE a;
	DATA_TYPE b;
	DATA_TYPE cr;
	DATA_TYPE ci;
	for (int i = indexBegin; i < indexEnd; i++) {
		screen[i] = 0;
		cr = complexNumbers[i].real;
		ci = complexNumbers[i].imaginary;
		for (int j = 0; j < ITERATIONS; j++) {
			a = complexNumbers[i].real;
			b = complexNumbers[i].imaginary;
			// Donne une autre fractale assez intéressante
			//complexNumbers[i].real = (a*a) - (b*b) + a;
			//complexNumbers[i].imaginary = (2 * a*b) + b;

			complexNumbers[i].real = (a*a) - (b*b) + cr;
			complexNumbers[i].imaginary = (2 * a*b) + ci;

			if ((complexNumbers[i].real*complexNumbers[i].real) + (complexNumbers[i].imaginary*complexNumbers[i].imaginary) > 4.0f) {
				screen[i] = (j + 1);
				break;
			}
		}
	}
	
}
*/

void Plane::setWindow(Vue v) {
	vue = v;
}

void Plane::drawPlane() {
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	resetComplexNumbers();
	for (int i = 0; i < NUM_THREAD; i++) {
		_thread[i] = thread(&Plane::leDessin, this, i * NUM_PIXEL / NUM_THREAD, (i + 1) * NUM_PIXEL / NUM_THREAD);
		SetThreadPriority(_thread[i].native_handle(), THREAD_PRIORITY_HIGHEST);
	}

	for (int i = 0; i < NUM_THREAD; i++) {
		_thread[i].join();
	}
	
	
	complexPlane.create(WIDTH, HEIGHT, (unsigned char*)screen);
	/*
	int k = 0;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			complexPlane.setPixel(j, i, screen[k++]);
		}
	}*/
	texture.loadFromImage(complexPlane);
	sprite.setTexture(texture);
	draw(sprite);
	if (ITERATIONS < 1500) ITERATIONS < 100 ? ITERATIONS += 1 : ITERATIONS *= 1.03;
	cout << "ITERATIONS : " << ITERATIONS << "\n";
}



void Plane::leDessin(int begin, int end) {

	// Calculate mandelbrot
	DATA_TYPE a;
	DATA_TYPE b;
	DATA_TYPE cr;
	DATA_TYPE ci;
	for (int i = begin; i < end; i++) {
		screen[i] = 0;
		cr = complexNumbers[i].real;
		ci = complexNumbers[i].imaginary;
		for (int j = 0; j < ITERATIONS; j++) {
			a = complexNumbers[i].real;
			b = complexNumbers[i].imaginary;
			// Donne une autre fractale assez intéressante
			//complexNumbers[i].real = (a*a) - (b*b) + a;
			//complexNumbers[i].imaginary = (2 * a*b) + b;

			complexNumbers[i].real = (a*a) - (b*b) + cr;
			complexNumbers[i].imaginary = (2 * a*b) + ci;

			if ((complexNumbers[i].real*complexNumbers[i].real) + (complexNumbers[i].imaginary*complexNumbers[i].imaginary) > 4.0f) {
				screen[i] = (j + 1);
				break;
			}
		}
	}

	// Shader
	for (int i = begin; i < end; i++) {
		// Do something with ComplexMandelbrot::count and screen pixel color
		float t, r, g, b;
		if (screen[i] != 0) {
			//screen[i] = (Color)( 0xFF0000FF + (complexNumbers[i].counter << 13));
			t =  (float)screen[i] / (float)ITERATIONS;
			b = 9 * (1 - t)*t*t*t;
			g = 15 * (1 - t)*(1 - t)*t*t;
			r = 8.5*(1 - t)*(1 - t)*(1 - t)*t;
			screen[i] = (static_cast<int>(r * 255)) + (static_cast<int>(g * 255) << 8) + (static_cast<int>(b * 255) << 16) + (static_cast<int>(255) << 24);
		}
	}
}


