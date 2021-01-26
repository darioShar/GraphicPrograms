#include <SFML\Graphics.hpp>
#include "Plane.h"
#include <limits>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace sf;

constexpr DATA_TYPE ZOOM_FACTOR = 1.3;

int main() {
	DATA_TYPE MOVE_FACTOR = 0.2;

	ofstream parametersFile;
	parametersFile.open("Parameters.txt", ios::app);
	if (!parametersFile.is_open()) {
		cout << "Couldn't open Paramters.txt\n";
	}

	Vue view(ASPECT_RATIO);

	// Pretty part in my taste (well, more or less...)
	/*
	view.xmin = -1.3930840090652123;
	view.xmax = -1.3929855686636550;
	view.ymin = -0.0057740940020837;
	view.ymax = -0.0057187212762075;
	*/

	// Middle of the biggest black thing, maximum calculations (every point is in the mandelbrot set).
	/*
	view.xmin = -0.5156385696821414;
	view.xmax = 0.2209879584352034;
	view.ymin = -0.2071762110330033;
	view.ymax = 0.2071762110330033;
	*/

	view.xmin = -0.1626159465168595;
	view.xmax = -0.1309984756283808;
	view.ymin = 0.8364693788801116;
	view.ymax = 0.8542542062548815;

	Plane plane(view);

	Event evnt;
	while (plane.isOpen()) {
		while (plane.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) {
				plane.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Z)) {
			view.move(0.0f, MOVE_FACTOR);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			view.move(0.0f, -MOVE_FACTOR);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			view.move(ASPECT_RATIO * (-MOVE_FACTOR), 0.0f);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			view.move(ASPECT_RATIO * MOVE_FACTOR, 0.0f);
			plane.setWindow(view);
		}
		if (Keyboard::isKeyPressed(Keyboard::E)) {
			view.zoom(ZOOM_FACTOR);
			plane.setWindow(view);
			MOVE_FACTOR /= ZOOM_FACTOR;
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			view.zoom(1.0f / ZOOM_FACTOR);
			plane.setWindow(view);
			MOVE_FACTOR *= ZOOM_FACTOR;
		}
		if (Mouse::isButtonPressed(Mouse::Button::Left)) {
			printf("Parameters :\n\
				Xmin = %.18lf,\n\
				Xmax = %.18lf,\n\
				Ymin = %.18lf,\n\
				Ymax = %.18lf\n\n\
				Mouse Cursor Position : %.18lf , %.18lf\n\n\
				Move factor : %.18lf\n\n\n",
				view.xmin,
				view.xmax,
				view.ymin,
				view.ymax,
				(DATA_TYPE)Mouse::getPosition().x * (view.xmax - view.xmin) / (DATA_TYPE)WIDTH,
				(DATA_TYPE)Mouse::getPosition().y * (view.ymax - view.ymin) / (DATA_TYPE)HEIGHT,
				MOVE_FACTOR);

			parametersFile << fixed << setprecision(numeric_limits<DATA_TYPE>::digits10 + 1) << "Parameters :\nview.xmin = " << view.xmin <<
				"\nview.xmax = " << view.xmax << "\nview.ymin = " << view.ymin << "\nview.ymax = " << view.ymax << "\n\nMouse Cursor : " <<
				(DATA_TYPE)Mouse::getPosition().x * (view.xmax - view.xmin) / (DATA_TYPE)WIDTH << " , " <<
				(DATA_TYPE)Mouse::getPosition().y * (view.ymax - view.ymin) / (DATA_TYPE)HEIGHT << "\n\n" <<
				"MOVE_FACTOR = " << MOVE_FACTOR << "\n\n\n";
		}
		
		plane.clear();
		plane.drawPlane();
		plane.display();
	}
	return 0;
}